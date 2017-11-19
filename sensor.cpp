#include "arduino.h"
#include "sensor.h"

SimpleDHT11 dht11;

Sensor::Sensor(int _pinData, int _umbral, Alarm *_alarma){
	pinData = _pinData;
  umbral = _umbral;
	alarma = _alarma;
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
    if (!alarma->fallaGlobal) { 
  	  // blinking sin usar delay
      if (millis() >> 9 & 1)
        alarma->activarLED();
      else
        alarma->desactivarLED();
    }
}

void Sensor::executeDesactivar()
{
	//Serial.println("execute desactivar");
 if (!desactivado) {
	  alarma->desactivarLED();
	  alarma->desactivarBuzzer();
    desactivado = true;
 }
}

void Sensor::executeAck()
{
	Serial.println("execute ack");
	alarma->desactivarBuzzer();
}

SensorLlama::SensorLlama(int _pinData, int _umbral, Alarm *_alarma):Sensor(_pinData, _umbral, _alarma){
  nombre = "Incendio";
  dweetCode = "S0";
}

void SensorLlama::modoNormal()
{
	Serial.println("execute normal");
  alarma->activarLED();
	alarma->activarBuzzer();
  alarma->enviarSMS("ALARMA INCENDIO ACTIVADA");
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
  // range value:
  
  switch (range) {
  case 0:    // A fire closer than 1.5 feet away.
    //Serial.println("** Close Fire **");
   return 1;
  case 1:    // A fire between 1-3 feet away.
    //Serial.println("** Distant Fire **");
    return 0;
  case 2:    // No fire detected.
    //Serial.println("No Fire");
    return 0;
  }
  
  return 0;
};

bool SensorLlama::hayFalla(){
  
  int range = this->leer();

  bool falla = range == 1;
  
  return falla;
};


DHT11::DHT11(int _pinData, int _umbral, Alarm *_alarma):Sensor(_pinData, _umbral, _alarma){
}

int DHT11::prev_temperatura;
int DHT11::prev_humedad;

void DHT11::modoNormal()
{
  Serial.println("execute normal");
  alarma->activarLED();
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
  dweetCode = "S1";
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
  dweetCode = "S2";
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
  dweetCode = "S3";
}

void SensorPIR::modoNormal()
{
  Serial.println("execute normal");
  alarma->activarLED();
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

  bool falla = range == 1;

  return falla;
};

//Sensor CO2
SensorCO2::SensorCO2(int _pinData, int _umbral, Alarm *_alarma):Sensor(_pinData, _umbral, _alarma){
  nombre = "Monoxido_Carbono";
  dweetCode = "S4";
}

void SensorCO2::modoNormal()
{
  Serial.println("execute normal");
  alarma->activarLED();
  alarma->activarBuzzer();
}

void SensorCO2::modoTest()
{

}

void SensorCO2::modoMant()
{

}


int SensorCO2::leer(){
  pinMode(pinData, INPUT);
  int sensorReading = analogRead(pinData);
  return sensorReading;
};

bool SensorCO2::hayFalla(){
  
  int range = this->leer();

  bool falla = false;

  if (range > umbral)
    falla = true;
  
  return falla;
};


