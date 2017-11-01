//include de librerias necesarias

#include <Keypad.h>
#include <Key.h>
#include <LiquidCrystal.h>
#include "config_pin.h"
#include "alarm.h"
#include "teclado.h"
// Fin include


//SENSORES
//sensorTemperatura =
//SensorLlama sensorLlama = SensorLlama(A0, 1);


//ALARMAS
//alarmaTemperatura = new AlarmaTemperatura(sensorTemperatura, lcd);
//AlarmaLlama alarmaLlama = AlarmaLlama(&sensorLlama);

Alarm *alarma;

//TECLADO
Keypad teclado = getKeypad();

void setup() {
	Serial.begin(9600);

	alarma = new Alarm();

}

void loop() {

	//Maneja el menu de opciones: cambio de modo, activacion/desactivacion de alarma, agregar sensores, etc.
	alarma->menu();

	//Verificar si la alarma esta activa
	if (alarma->getEnable()){
		alarma->verificarSensores();
	}

	/*	//logica para leer codigos y cambiar modo normal, test, mantenimiento
	char customKey = teclado.getKey();

	if (customKey) {
		Serial.println(customKey);
	}

	//alarmas
	//falla = sensorTemperatura.hayFalla();
	//alarmaTemperatura.execute(enable, falla, ack, modo);

	//bool falla = sensorLlama.hayFalla();
	//alarmaLlama.check(enable, falla, ack, modo);*/

}