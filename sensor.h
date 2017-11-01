#ifndef _SENSOR_h
#define _SENSOR_h

#include "arduino.h"
#include "alarm.h"
class Alarm;

class Sensor {
   protected:
	   int pinData;
	   int zona;
	   double umbral;
	   Alarm *alarma;

   public:
      Sensor(int pinData, int  umbral, Alarm *_alarma);
	  virtual void executeNormal();
	  virtual void executeTest();
	  virtual void executeMant();
      virtual bool hayFalla();
};

class SensorLlama : public Sensor {
  public:
     SensorLlama(int _pinData, int _umbral, Alarm *_alarma);
	 void executeNormal();
	 void executeTest();
	 void executeMant();
     bool hayFalla();
};

#endif
