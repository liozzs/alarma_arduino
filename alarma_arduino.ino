//include de librerias necesarias

#include <Keypad.h>
#include <Key.h>
#include <LiquidCrystal.h>
#include "config_pin.h"
#include "alarm.h"
#include "teclado.h"
// Fin include


Alarm *alarma;


void setup() {
	Serial.begin(9600);

	alarma = new Alarm();

	// Agregamos sensores de forma manual
	alarma->addSensor(new SensorLlama(PIN_SENSOR_LLAMA, 1, alarma));

}

void loop() {

	//Maneja el menu de opciones: cambio de modo, activacion/desactivacion de alarma, agregar sensores, etc.
	alarma->menu();

	//Verificar estados sensores
	alarma->verificarSensores();
	
	delay(200);
}