//include de librerias necesarias

#include <LiquidCrystal.h>
#include "config_pin.h"
#include "sensor.h"
#include "alarm.h"
#include "teclado.h"
// Fin include

//SENSORES
//sensorTemperatura =
SensorLlama sensorLlama = SensorLlama(A0, 1);

//DISPLAY
//lcd = 

//ALARMAS
//alarmaTemperatura = new AlarmaTemperatura(sensorTemperatura, lcd);
AlarmaLlama alarmaLlama =  AlarmaLlama(&sensorLlama);

//TECLADO
Keypad teclado = getKeypad();

void setup() {
  Serial.begin(9600);
}

void loop() {
  //SET pin modes
  pinMode(PIN_ENABLE, INPUT);
  pinMode(PIN_ACK, INPUT);

  //leer teclado

  //logica para leer codigos y cambiar modo normal, test, mantenimiento
  char customKey = teclado.getKey();
  
  if (customKey){
    Serial.println(customKey);
  }

  //logica para determinar modo
  String modo = "normal";

  //leer entradas
  bool enable = digitalRead(PIN_ENABLE);
  bool ack =  digitalRead(PIN_ACK);
  enable = true;
  //alarmas
  //falla = sensorTemperatura.hayFalla();
  //alarmaTemperatura.execute(enable, falla, ack, modo);

  bool falla = sensorLlama.hayFalla();
  alarmaLlama.check(enable, falla, ack, modo);
  delay(100);
}





