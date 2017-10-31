#ifndef _SENSOR_h
#define _SENSOR_h

#include "arduino.h"
#include "alarm.h"
class Alarm;

class Sensor {
   private:
      
   public:
      int pinData;
	  int zona;
      double umbral;
	  Alarm *alarma;

      Sensor(int pinData, int  umbral, Alarm *_alarma);
      bool hayFalla();
};

class SensorLlama : public Sensor {
  public:
     SensorLlama(int _pinData, int _umbral, Alarm *_alarma);
     bool hayFalla();
};

#endif
