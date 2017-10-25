#ifndef sensor_h
#define sensor_h

#include "Arduino.h"


class Sensor {
   private:
      
   public:
      int pinData;
      double umbral;
      Sensor(int pinData, int  umbral);
      bool hayFalla();
};

class SensorLlama : public Sensor {
  public:
     SensorLlama(int pinData, int  umbral);
     bool hayFalla();
};

#endif
