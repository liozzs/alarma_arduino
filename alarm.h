#ifndef _ALARM_h
#define _ALARM_h

#include "arduino.h"
#include "status.h"
#include "sensor.h"
#include "config_pin.h"
#include <LiquidCrystal.h>
#include <LinkedList.h>

//Modos de operacion
#define MODO_NORMAL 1
#define MODO_TEST	2
#define MODO_MANT	3

//Estados
#define EST_ACTIVA	 1
#define EST_DESACT	 2
#define EST_ACK		 3
#define EST_ALARMADA 4

class Sensor;

class Alarm{

   protected:
      int modoOperacion;
      LiquidCrystal *lcd;
      int estado;
	  bool ack = false, estado_ack = false, estado_falla = false;
	  unsigned long lastPeriodStart = 0;
	  const int onDuration = 500;
	  const int periodDuration = 800;
	  LinkedList<Sensor*> sensores = LinkedList<Sensor*>();
   public:
      
      Alarm();
	  void menu();
	  bool getEnable();
	  int getMode();
	  void verificarSensores();
	  void activarBuzzer();
	  void desactivarBuzzer();
	  void addSensor(Sensor*);
};


#endif
