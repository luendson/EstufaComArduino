#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

// Credenciais Wi-Fi
const char* ssid = "*********";
const char* password = "*********";

// Configurações MQTT
const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;
const char* mqtt_client_id = "*********"; // Identificador do cliente MQTT

bool conectado = false; // Indica o estado da conexão MQTT

// Objetos
WiFiClient **********;
PubSubClient client(************);

#define pinoDHT 4
#define tipoDHT DHT22
DHT dht(pinoDHT, tipoDHT);
int temperaturaInteira;
float umidadePorcentagem;
const int pinoRele = 21;


void setup() 
{
  dht.begin();//sensor DHT]
  pinMode(pinoRele, OUTPUT);
  //client.subscribe(mqtt_topic);
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("."); // Imprime um ponto para indicar que ainda está tentando se conectar
  }
  Serial.println(""); // Imprime uma linha em branco
  Serial.println("WiFi conectado!"); // Informa que a conexão Wi-Fi foi bem-sucedida
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  if (client.connect(mqtt_client_id)) {
    client.subscribe("oq");
    Serial.println("Conectado ao MQTT!-");
  } else {
    Serial.print("Falha ao conectar ao MQTT, rc=");
    Serial.println(client.state());
  }
}

void loop() 
{
  
  
  if (!client.connected()) {
    // Se não estiver conectado, tenta se reconectar
    if (reconnect()) 
    {
      client.publish("esp32/verificacao", "conectado");
      client.subscribe("oq");
    } else {
      Serial.print("Falha ao conectar no MQTT, tentando novamente em 5 segundos..."); // Informa que a conexão MQTT falhou e tenta novamente em 5 segundos
      digitalWrite(13, HIGH);
      delay(5000); // Aguarda 5 segundos antes de tentar novamente
    }
  }
  client.loop();
  SensorAr();
  delay(1000);
}

void callback(char* topic, byte* payload, unsigned int length) 
{
  Serial.print("message recebiada");
  Serial.print("topic");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  if ((char)payload[0] == 's'){
    Regar();
  }
}

void Regar()
{
  digitalWrite(pinoRele, HIGH);
  Serial.print("regando");
  delay(10000);
  digitalWrite(pinoRele, LOW);
  Serial.print("regando--FIM");
}

void SensorAr()
{
  // Ler dados do sensor
  float temperaturaFloat = dht.readTemperature();
  float umidadeFloat = dht.readHumidity();

  // Verificar erros de leitura
  if (isnan(temperaturaFloat) || isnan(umidadeFloat)) {
    Serial.println("Falha ao ler dados do sensor!");
    return;
  }

  // Converter para inteiro
  temperaturaInteira = (int)round(temperaturaFloat);
  umidadePorcentagem = (int)round(umidadeFloat);

  // Exibir valores no monitor serial
  Serial.print("Temperatura: ");
  Serial.print(temperaturaInteira);
  Serial.print("°C");
  Serial.print(" | Umidade: ");
  Serial.print(umidadePorcentagem);
  Serial.println("%");

  String temperaturaString = String(temperaturaInteira);
  client.publish("esp32/temperatura", temperaturaString.c_str());
  
  String umidadeString = String(umidadePorcentagem);
  client.publish("esp32/umidadeAr", umidadeString.c_str());
  
}

boolean reconnect() 
{
  if (client.connect("************")) {
    return true; // Retorna true se a conexão for bem-sucedida
  } else {
    return false; // Retorna false se a conexão falhar
  }
}