#ifndef _SENSOR_h
#define _SENSOR_h

#include "arduino.h"
#include "alarm.h"
#include <SimpleDHT.h>

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
    String nombre;
    Sensor(int pinData, int  umbral, Alarm *_alarma);
    void executeNormal();
    void executeBlink();
    void executeDesactivar();
    void executeAck();
    virtual bool hayFalla() = 0;
    virtual void modoNormal() = 0;
    virtual void modoTest() = 0;
    virtual void modoMant() = 0;
    virtual int leer() = 0;
};

class SensorLlama : public Sensor {

  public:
    SensorLlama(int _pinData, int _umbral, Alarm *_alarma);
    void modoNormal();
    void modoTest();
    void modoMant();
    bool hayFalla();
    int leer();
};

class DHT11 : public Sensor {


  public:
    static int prev_temperatura;
    static int prev_humedad;
    DHT11(int _pinData, int _umbral, Alarm *_alarma);
    void modoNormal();
    void modoTest();
    void modoMant();
    bool hayFalla();
    virtual int leer();
};

class SensorTemperatura : public DHT11 {
  public:
    SensorTemperatura(int _pinData, int _umbral, Alarm *_alarma);
    int leer();
};

class SensorHumedad : public DHT11 {
  public:
    SensorHumedad(int _pinData, int _umbral, Alarm *_alarma);
    int leer();
};

class SensorPIR : public Sensor {

  public:
    SensorPIR(int _pinData, int _umbral, Alarm *_alarma);
    void modoNormal();
    void modoTest();
    void modoMant();
    bool hayFalla();
    int leer();
};
#endif
