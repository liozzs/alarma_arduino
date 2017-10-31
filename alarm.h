#ifndef _ALARM_h
#define _ALARM_h

#include "arduino.h"
#include "status.h"
#include "sensor.h"
#include "config_pin.h"
#include <LiquidCrystal.h>
#include <LinkedList.h>

class Sensor;

class Alarm{

   protected:
      int modoOperacion;
      LiquidCrystal *lcd;
      bool estado_falla;
      bool estado_ack;
	  LinkedList<Sensor*> sensores = LinkedList<Sensor*>();
   public:
      
      Alarm();
	  void menu();
	  bool getEnable();
	  int getMode();
	  /*
      virtual void executeNormal();
      virtual void executeAck();
      virtual void executeBlink();
      virtual void executeDesactivar();
      void mostrarEstado();
      void getInfoSensor(); */
      void check(bool enable, bool fail, bool ack, String mode);

};

class AlarmaLlama : public Alarm {
  private:
      unsigned long lastPeriodStart;
      const int onDuration=500;
      const int periodDuration=800;
  public:
   AlarmaLlama();

   void executeNormal();
      void executeAck();
      void executeBlink();
      void executeDesactivar();
     // void mostrarEstado();
     //void getInfoSensor();
      void check(bool enable, bool fail, bool ack, String mode);
};

#endif
