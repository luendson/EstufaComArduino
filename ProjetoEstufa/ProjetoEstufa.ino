#define pinBotao 3
#include <dht.h>

#include <LiquidCrystal_I2C.h>
#define endereco  0x27 // Endereços comuns: 0x27, 0x3F
#define colunas   16
#define linhas    2
LiquidCrystal_I2C lcd(endereco, colunas, linhas);//configuracao de tela

#define sensorDoAr 5
#define sensorDeLuz A0

unsigned long tempoInicio = 0;
unsigned long tempoBotao = 0;
bool estadoBotao;
bool estadoBotaoAnt;//para o uso do botao

dht my_dht;


int temperatura = 0x00,
    umidade = 0x00;



void setup() {
  pinMode(pinBotao, INPUT_PULLUP);
  Serial.begin(9600);
  Serial.print("aperte o botao para iniciar\n");
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.print(" aperte o botao");
  lcd.setCursor(0, 1);
  lcd.print("  para iniciar");
}

bool painell = false;
int tempo = 0;
int controle = 0;

void loop() {
  my_dht.read11(sensorDoAr);
  estadoBotao = !digitalRead(pinBotao);

  //Ao apertar o botão
  if (estadoBotao && !estadoBotaoAnt) {
    if (tempoInicio == 0) {
      tempoInicio = millis();
      painell = false;
      controle = 0;
      tempo = 0;
    }
  }  
  //Ao soltar o botão
  if (tempoInicio > 200) {  //Filtro Debounce
    if (!estadoBotao && estadoBotaoAnt) {
      tempoBotao = millis() - tempoInicio;
      tempoInicio = 0;
      painell = true;
    }
  }
  //Menu para exibir sensor especifico 
  if (tempoInicio > 0) {
    tempo = millis() - tempoInicio;
    if((tempo > 0) && (tempo < 1000)){
      if(controle == 0){
        Serial.print("Umidade do Ar \n");
        lcd.clear();
        lcd.print("Umidade do Ar");
        controle = 1; 
      }
    }
    if((tempo > 1000) && (tempo < 2000)){
      if(controle == 1){
        Serial.print("temperatura do Ar \n");
        lcd.setCursor(0, 0);
        lcd.print("temperatura Ar");
        controle = 2; 
      }
    }
    if((tempo > 2000) && (tempo < 3000)){
      if(controle == 2){
        Serial.print("luz do ambiente \n");
        lcd.clear();
        lcd.print("luz do ambiente");
        controle = 3; 
      }
      
    }
  }
  if (painell == true){//chamando funcao exibir
    if(controle == 1){
      Serial.print("umidade = ");
      umidade = LerSensor("umidade");
      Serial.print(umidade);
      Serial.println("%");
      mostra(1);
    }
    if(controle == 2){
      Serial.print("temperatura = ");
      temperatura = LerSensor("temperatura");
      Serial.print(temperatura);
      Serial.println("C");
      mostra(2);
    }
    if(controle == 3){
      int Luz = LerSensor("luz");
      Serial.print("Luz = ");  
      Serial.println(Luz);
      mostra(3);
    }
  }
  estadoBotaoAnt = estadoBotao;
  delay(20);
}

int mostra(char* dados){//exibir sensor
  lcd.clear();
  if (dados == 1){
    lcd.print(LerSensor("umidade"));
    lcd.print("%'");
  }
  if (dados == 2){
    lcd.print(LerSensor("temperatura"));
    lcd.print("C");
  }
  if (dados == 3){
    lcd.print(LerSensor("luz"));
    lcd.print(" Luz");
  }

}

int LerSensor(char* sensor) {//sensores 
  if(sensor == "luz"){
    int LuzAnalogica = analogRead(sensorDeLuz);
    int LuzPorcetagem = map(LuzAnalogica, 0, 1023, 0, 100);
    return LuzPorcetagem;
  }
  if(sensor == "umidade"){
    umidade = my_dht.humidity;
    return umidade;
  }
  if(sensor == "temperatura"){
    temperatura = my_dht.temperature;
    return temperatura;
  } 
}



