#include <dht.h>

#define sensorDoAr 5
#define sensorDeLuz A0

dht my_dht;

int temperatura = 0x00,
    umidade = 0x00;

void setup() {
  Serial.begin(9600);
}

void loop() {
  my_dht.read11(sensorDoAr);
  delay(1500);
  
  Serial.print("temperatura = ");
  temperatura = LerSensor("temperatura");
  Serial.print(temperatura);
  Serial.println("C");
  
  Serial.print("umidade = ");
  umidade = LerSensor("umidade");
  Serial.print(umidade);
  Serial.println("%");

  int Luz = LerSensor("luz");
  Serial.print("Luz = ");  
  Serial.println(Luz);
  Serial.println("-----------------------");
}

int LerSensor(char* sensor) {
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
