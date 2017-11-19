//include de librerias necesarias

#include <Keypad.h>
#include <Key.h>
#include <LiquidCrystal.h>
#include "config_pin.h"
#include "alarm.h"
#include "conectividad.h"
// Fin include



Alarm *alarma;
unsigned long tiempoAnterior = 0; 


void setup() {
	Serial.begin(115200);  //Monitor Serial
  Serial1.begin(115200); //Comunicacion con ESP8266
  Serial2.begin(115200); //Comunicacion con SIM900


  Serial2.println("AT+CREG=1");
    
	alarma = new Alarm();

	// Agregamos sensores de forma manual
	alarma->addSensor(new SensorLlama(PIN_SENSOR_LLAMA, 1, alarma));
  alarma->addSensor(new SensorTemperatura(PIN_SENSOR_TEMPERATURA, 40, alarma));
  alarma->addSensor(new SensorHumedad(PIN_SENSOR_TEMPERATURA, 80, alarma));
  alarma->addSensor(new SensorPIR(PIN_SENSOR_PIR, 50, alarma));
  alarma->addSensor(new SensorCO2(PIN_SENSOR_CO2, 250, alarma));


  delay(3000); //delay para que inicialice el WIFI
  //sendToWIFI2("modo:TEST");
 
}

 String readString = "";

void loop() {

  //while (Serial1.available())
   // Serial.write(Serial1.read());
    
  //leer mensajes de control desde dashboard
  alarma->procesarAcciones();
  
	//Maneja el menu de opciones: cambio de modo, activacion/desactivacion de alarma, agregar sensores, etc.
	 alarma->mostrarMenu();

	//Verificar estados sensores
	alarma->verificarSensores();
  

  if(millis()>tiempoAnterior + PERIODO_DWEET){ 
    //Serial.println("Enviando estado a Dweet");
    alarma->enviarEstado();
    tiempoAnterior=millis();  
  }
	
	delay(200);
}


