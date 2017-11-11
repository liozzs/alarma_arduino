#include "arduino.h"
#include "sensor.h"

Sensor::Sensor(int _pinData, int _umbral, Alarm *_alarma){
	pinData = _pinData;
    umbral = _umbral;
	alarma = _alarma;

	led = LED;
}

void Sensor::executeNormal()
{

	//Ejecutar de acuerdo al modo de operacion activo
	switch (alarma->getMode()) {

	case MODO_NORMAL:
		this->modoNormal();
		break;

	case MODO_TEST:
		this->modoTest();
		break;

	case MODO_MANT:
		this->modoMant();
		break;
	}

}

void Sensor::executeBlink()
{
	Serial.println("execute blink");
	digitalWrite(led, millis() >> 9 & 1); // blinking sin usar delay
}

void Sensor::executeDesactivar()
{
	Serial.println("execute desactivar");
	digitalWrite(led, LOW);
	alarma->desactivarBuzzer();
}

void Sensor::executeAck()
{
	Serial.println("execute ack");
	alarma->desactivarBuzzer();
}

SensorLlama::SensorLlama(int _pinData, int _umbral, Alarm *_alarma):Sensor(_pinData, _umbral, _alarma){

}

void SensorLlama::modoNormal()
{
	Serial.println("execute normal");

	digitalWrite(led, HIGH);

	alarma->activarBuzzer();
}

void SensorLlama::modoTest()
{

}

void SensorLlama::modoMant()
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

