#include "arduino.h"
#include "alarm.h"

Alarm::Alarm(){
	   
	//SET pin modes
	pinMode(PIN_ENABLE, INPUT);
	pinMode(PIN_ACK, INPUT);

	pinMode(LED, OUTPUT);
	pinMode(BUZZER, OUTPUT);

	this->modoOperacion = MODO_NORMAL;
}

void Alarm::menu()
{
	ack = !digitalRead(PIN_ACK);
}

bool Alarm::getEnable()
{
	int enable = digitalRead(PIN_ENABLE);
	
	if (enable == HIGH)
		return true;
	else
		return true;
}

int Alarm::getMode()
{
	return modoOperacion;
}

void Alarm::verificarSensores()
{
	Sensor *sensor;
	bool falla;
	bool changeAck = false;

	// guardo cuando se hizo ack
	if (ack == true) {
		estado_ack = true;
		estado_falla = false;
		ack = false;
	}
	
	changeAck = !estado_ack;

	if (this->getEnable()){

		//Recorrer lista de sensores
		for(int i = 0; i < sensores.size(); i++) {

			sensor = sensores.get(i);	//Obtener sensor de la lista

			falla = sensor->hayFalla();

			if (falla && !estado_ack)
				sensor->executeNormal();
			else if (falla && estado_ack)
				sensor->executeAck();
			else if (!falla) {
				if (sensor->estado_falla)
					sensor->executeBlink();
				else {
					changeAck = true;
					sensor->executeDesactivar();
				}
			}
			//guardo estado previo de falla
			if (falla == true && changeAck){
				sensor->estado_falla = true;
				estado_falla = true;
			}
		}
		if (changeAck)				//Actualizar estado ack
			estado_ack = false;
	}
}

void Alarm::activarBuzzer()
{
	if (millis() - lastPeriodStart >= periodDuration)
	{
		lastPeriodStart += periodDuration;
		tone(BUZZER, 800, onDuration); // play 800 Hz tone in background for 'onDuration'
	}
}

void Alarm::desactivarBuzzer()
{
	noTone(BUZZER);
}

void Alarm::addSensor(Sensor* _sensor)
{
	sensores.add(_sensor);
}
