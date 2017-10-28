#ifndef Alarm_h
#define Alarm_h

#include "Arduino.h"
#include "Status.h"
#include "sensor.h"
#include <LiquidCrystal.h>

class Alarm{

   protected:
      Sensor sensor = sensor;
      char*  modo_alarma;
      LiquidCrystal lcd = lcd;
      bool estado_falla;
      bool estado_ack;
   public:
      // Funciones miembro de la clase "pareja"
      Alarm(Sensor* sensor);
      virtual void executeNormal();
      virtual void executeAck();
      virtual void executeBlink();
      virtual void executeDesactivar();
      void mostrarEstado();
      void getInfoSensor();
      void check(bool enable, bool fail, bool ack, String mode);

};

class AlarmaLlama : public Alarm {
  private:
      unsigned long lastPeriodStart;
      const int onDuration=500;
      const int periodDuration=800;
  public:
   AlarmaLlama(Sensor* sensor);

   void executeNormal();
      void executeAck();
      void executeBlink();
      void executeDesactivar();
      void mostrarEstado();
      void getInfoSensor();
      void check(bool enable, bool fail, bool ack, String mode);
};

#endif
