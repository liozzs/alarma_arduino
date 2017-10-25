#ifndef Alarm_h
#define Alarm_h

#include "Arduino.h"
#include "Status.h"
#include "sensor.h"
#include <LiquidCrystal.h>

class Alarm{
   private:
      Sensor sensor = sensor;
      char*  modo_alarma;
      LiquidCrystal lcd = lcd;
      bool estado_falla;
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
