
#include <dht.h>

//sensor do ar responsavel por temperatura e humidade do ar DHT11;
#define sensorDoAr 5
dht my_dht;
//sensor de luz
#define sensorDeLuz A0

//variaveis responasevei por guardar dados dos leitores;
int temperatura = 0x00,
umidade = 0x00;

void setup() {
  Serial.begin(9600);
}

void loop() {
  //convertendo umidade e temperatura;
  my_dht.read11(sensorDoAr);
  temperatura = my_dht.temperature;
  umidade = my_dht.humidity;
  //leitura da porta A0 sensor de luz;
  int Luz = map(analogRead(sensorDeLuz), 0, 1023, 0, 100); // Mapear para a faixa de 0 a 100

  delay(1500);
  //teste
  
  Serial.print("temperatura = ");
  Serial.print(temperatura);
  Serial.print("C");
  Serial.print(" \n");
  
  Serial.print("umidade = ");
  Serial.print(umidade);
  Serial.print("%");
  Serial.print(" \n");

  Serial.print("Luz = ");  
  Serial.print(Luz);
  Serial.print(" \n");
  Serial.print(("----------------------- \n"));

}
