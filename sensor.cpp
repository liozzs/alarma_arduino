#include "arduino.h"
#include "sensor.h"

SimpleDHT11 dht11;

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
	//Serial.println("execute desactivar");
	digitalWrite(led, LOW);
	alarma->desactivarBuzzer();
}

void Sensor::executeAck()
{
	Serial.println("execute ack");
	alarma->desactivarBuzzer();
}

SensorLlama::SensorLlama(int _pinData, int _umbral, Alarm *_alarma):Sensor(_pinData, _umbral, _alarma){
  nombre = "Incendio";
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

//en este caso retorna 1 si hay fuego cerca o distante, 0 si no hay
int SensorLlama::leer(){
  // read the sensor on analog A0:
  int sensorReading = analogRead(pinData);
  const int sensorMin = 0;     // sensor minimum
  const int sensorMax = 1024; 

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
    //Serial.println("** Close Fire **");
    falla = true;
    break;
  case 1:    // A fire between 1-3 feet away.
    //Serial.println("** Distant Fire **");
    falla = false;
    break;
  case 2:    // No fire detected.
    //Serial.println("No Fire");
    falla = false;
    break;
  }

  return falla;
};


DHT11::DHT11(int _pinData, int _umbral, Alarm *_alarma):Sensor(_pinData, _umbral, _alarma){
}

int DHT11::prev_temperatura;
int DHT11::prev_humedad;

void DHT11::modoNormal()
{
  Serial.println("execute normal");
  digitalWrite(led, HIGH);
  alarma->activarBuzzer();
}

void DHT11::modoTest()
{

}

void DHT11::modoMant()
{

}

bool DHT11::hayFalla(){
  
  int range = this->leer();

  bool falla = false;
  if (range > umbral) {
    falla = true;
  }

  return falla;
};

SensorTemperatura::SensorTemperatura(int _pinData, int _umbral, Alarm *_alarma):DHT11(_pinData, _umbral, _alarma){
  nombre = "Temperatura";
}

//retorna -1 si no pudo medir
int SensorTemperatura::leer(){
  // read without samples.
  byte temperature = 0;
  byte humidity = 0;
  int err = SimpleDHTErrSuccess;
  if ((err = dht11.read(pinData, &temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
    //el sensor devuelve data cada 1 segundo, en caso de que no este listo retorno el valor anterior.
    return DHT11::prev_temperatura;
  }
  DHT11::prev_temperatura = temperature;
  DHT11::prev_humedad = humidity;
  return temperature;
};

SensorHumedad::SensorHumedad(int _pinData, int _umbral, Alarm *_alarma):DHT11(_pinData, _umbral, _alarma){
  nombre = "Humedad";
}

//retorna -1 si no pudo medir
int SensorHumedad::leer(){
  // read without samples.
  byte temperature = 0;
  byte humidity = 0;
  int err = SimpleDHTErrSuccess;
  if ((err = dht11.read(pinData, &temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
    //el sensor devuelve data cada 1 segundo, en caso de que no este listo retorno el valor anterior.
    return DHT11::prev_humedad;
  }

  DHT11::prev_temperatura = temperature;
  DHT11::prev_humedad = humidity;
  return humidity;

};

SensorPIR::SensorPIR(int _pinData, int _umbral, Alarm *_alarma):Sensor(_pinData, _umbral, _alarma){
  nombre = "Movimiento";
}

void SensorPIR::modoNormal()
{
  Serial.println("execute normal");

  digitalWrite(led, HIGH);

  alarma->activarBuzzer();
}

void SensorPIR::modoTest()
{

}

void SensorPIR::modoMant()
{

}

//en este caso retorna 1 si hay fuego cerca o distante, 0 si no hay
int SensorPIR::leer(){

  pinMode(pinData, INPUT);
  int sensorReading = digitalRead(pinData);
  const int sensorMin = 0;     // sensor minimum
  const int sensorMax = 1024; 

  return sensorReading;
};

bool SensorPIR::hayFalla(){
  
  int range = this->leer();

  bool falla = false;

  return falla;
};


