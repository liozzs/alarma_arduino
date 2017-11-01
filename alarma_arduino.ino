//include de librerias necesarias

#include <LiquidCrystal.h>
#include "config_pin.h"
#include "sensor.h"
#include "alarm.h"
//#include "teclado.h"
#include "conectividad.h"
#include "dweet.h"
// Fin include

//SENSORES
SensorTemperatura sensorTemperatura = SensorTemperatura(SENSOR_TEMPERATURA, 35);
SensorLlama sensorLlama = SensorLlama(SENSOR_LLAMA, 1);

//DISPLAY
//lcd = 

//ALARMAS
AlarmaTemperatura alarmaTemperatura =  AlarmaTemperatura(&sensorTemperatura);
AlarmaLlama alarmaLlama =  AlarmaLlama(&sensorLlama);

//TECLADO
//Keypad teclado = getKeypad();

//CONECTIVIDAD
WiFiEspClient clienteWiFi;
Dweet dweet(clienteWiFi);

//GLOBALES
String modo;
bool enable;
bool ack;
bool falla;

void setup() {
  Serial.begin(9600);

  //Inicializacion
   clienteWiFi = getClienteWiFi();

  //Initialize the Dweet client library
  Dweet dweet(clienteWiFi);

  //Interrupciones
  //pinMode(3, INPUT_PULLUP);
  //attachInterrupt( digitalPinToInterrupt(3), handlerInt, FALLING);
  
  //SET pin modes
  pinMode(PIN_ENABLE, INPUT);
  pinMode(PIN_ACK, INPUT);
  pinMode(LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
}

//para medir el tiempo y ejecutar GET y POST 
long previousMillis = 0;

void loop() {
  unsigned long currentMillis = millis();

  //leet dweet para controlar alguna accion en la alarma
  //enable, ack
//  Serial.println(currentMillis - previousMillis);
//  if(currentMillis - previousMillis > DELAY_GET) {
//  String control = dweet.get_latest("frba-ssee-alarma-controlback");
//  Serial.println("Recibiendo info de dweet");
//  Serial.println(control);
//  previousMillis = millis();
//  }
  
  //leer teclado

  //logica para leer codigos y cambiar modo normal, test, mantenimiento
  //char customKey = teclado.getKey();
  
  //if (customKey){
    //Serial.println(customKey);
  //}

  //logica para determinar modo
  modo = "normal";

  //leer entradas
  enable = digitalRead(PIN_ENABLE);
  ack =  !digitalRead(PIN_ACK);

  enable = true;
  
  //alarmas
  falla = sensorTemperatura.hayFalla(); 
  alarmaTemperatura.check(enable, falla, ack, modo);

  falla = sensorLlama.hayFalla();
  alarmaLlama.check(enable, falla, ack, modo);

  //envio de info a dweet
  if(currentMillis - previousMillis > DELAY_POST) {
    Sensor* sensores[] = {&sensorLlama, &sensorTemperatura};
    enviarDweets(2, sensores);
    previousMillis = millis();
  }
  

  delay(MAIN_LOOP_DELAY);
}

//Envia los datos de los sensores a dweet
void enviarDweets(int number, Sensor* sensores[]){
    for(int i=0; i<number; i++){
        Serial.println("Medicion :");
        Serial.println(sensores[i]->name);
        Serial.println(sensores[i]->leer());
    }
}

void handlerInt() 
   {   Serial.println("ALARMA!!!!!!");
     bool falla = sensorLlama.hayFalla();
     alarmaLlama.check(true, falla, false, "normal");
   }





