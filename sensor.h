#ifndef _SENSOR_h
#define _SENSOR_h

#include "arduino.h"
#include "alarm.h"
class Alarm;

class Sensor {
   protected:
	   int pinData;
	   int zona;
	   int led;
	   double umbral;
	   Alarm *alarma;

   public:
	  bool estado_falla = false;

      Sensor(int pinData, int  umbral, Alarm *_alarma);
	  void executeNormal();
	  void executeBlink();
	  void executeDesactivar();
	  void executeAck();
      virtual bool hayFalla();	  
	  virtual void modoNormal();
	  virtual void modoTest();
	  virtual void modoMant();
};

class SensorLlama : public Sensor {
  public:
     SensorLlama(int _pinData, int _umbral, Alarm *_alarma);
	 void modoNormal();
	 void modoTest();
	 void modoMant();
     bool hayFalla();
};

#endif
