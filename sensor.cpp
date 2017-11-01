#include "arduino.h"
#include "sensor.h"

Sensor::Sensor(int _pinData, int _umbral, Alarm *_alarma){
	pinData = _pinData;
    umbral = _umbral;
	alarma = _alarma;
}

SensorLlama::SensorLlama(int _pinData, int _umbral, Alarm *_alarma):Sensor(_pinData, _umbral, _alarma) {

}

void SensorLlama::executeNormal()
{
	if (this->hayFalla) {

		alarma->activarBuzzer();
	}

}

void SensorLlama::executeTest()
{

}

void SensorLlama::executeMant()
{

}

bool SensorLlama::hayFalla(){
  // read the sensor on analog A0:
  int sensorReading = analogRead(pinData);
  const int sensorMin = 0;     // sensor minimum
  const int sensorMax = 1024;  // sensor maximum
  // map the sensor range (four options):
  // ex: 'long int map(long int, long int, long int, long int, long int)'
  int range = map(sensorReading, sensorMin, sensorMax, 0, 3);

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
}

