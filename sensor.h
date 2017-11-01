#ifndef sensor_h
#define sensor_h

#include "Arduino.h"
#include <SimpleDHT.h>


class Sensor {
   private:
      
   public:
      char *name = "Sensor Llama";
      int pinData;
      double umbral;
      Sensor(int pinData, int  umbral);
      bool hayFalla();
      virtual int leer();
};

class SensorLlama : public Sensor {
  private:
   const int sensorMin = 0;     // sensor minimum
   const int sensorMax = 1024;  // sensor maximum
  
  public:
     SensorLlama(int pinData, int  umbral);
     bool hayFalla();
     virtual int leer();
};

class SensorTemperatura : public Sensor {

  public:
     SensorTemperatura(int pinData, int  umbral);
     bool hayFalla();
     virtual int leer();
};

#endif

