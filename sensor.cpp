#include "Arduino.h"
#include "Sensor.h"

SimpleDHT11 dht11;

Sensor::Sensor(int pinData, int umbral){
       pinData = pinData;
       umbral = umbral;
}

 int Sensor:: leer(){
  
};

SensorLlama::SensorLlama(int pinData, int umbral) : Sensor(pinData, umbral) {
};

SensorTemperatura::SensorTemperatura(int pinData, int umbral) : Sensor(pinData, umbral) {
};

//en este caso retorna 1 si hay fuego cerca o distante, 0 si no hay
int SensorLlama::leer(){
  // read the sensor on analog A0:
  int sensorReading = analogRead(pinData);

  // map the sensor range (four options):
  // ex: 'long int map(long int, long int, long int, long int, long int)'
  int range = map(sensorReading, sensorMin, sensorMax, 0, 3);
  return range;
};

bool SensorLlama::hayFalla(){
  
  int range = this->leer();

  bool falla = false;
  // range value:
  switch (range) {
  case 0:    // A fire closer than 1.5 feet away.
    Serial.println("** Close Fire **");
    falla = true;
    break;
  case 1:    // A fire between 1-3 feet away.
    Serial.println("** Distant Fire **");
    falla = false;
    break;
  case 2:    // No fire detected.
    Serial.println("No Fire");
    falla = false;
    break;
  }

  return falla;
};

//retorna -1 si no pudo medir
int SensorTemperatura::leer(){
  // read without samples.
  byte temperature = 0;
  byte humidity = 0;
  int err = SimpleDHTErrSuccess;
  if ((err = dht11.read(pinData, &temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
    Serial.print("Read DHT11 failed, err="); 
    Serial.println(err);
    return -1;
  }
  return temperature;
};

bool SensorTemperatura::hayFalla(){
  
  int range = this->leer();

  bool falla = false;
  if (range > umbral) {
    falla = true;
  }

  return falla;
};


