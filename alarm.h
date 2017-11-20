#ifndef _ALARM_h
#define _ALARM_h

#include "arduino.h"
#include "sensor.h"
#include "config_pin.h"
#include <LiquidCrystal.h>
#include <LinkedList.h>
#include <Keypad.h>

//Modos de operacion
#define MODO_NORMAL 1
#define MODO_TEST	2
#define MODO_MANT	3

//Estados
#define EST_ACTIVA	 1
#define EST_DESACT	 2
#define EST_ACK		 3
#define EST_ALARMADA 4

//LCD
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5
#define btnEMPTY  99

//MENU
#define ACTIVAR	   1
#define DESACTIVAR 2

class Sensor;

class Menu;

class Alarm{

   protected:
    int modoOperacion;
    LiquidCrystal *lcd;
    int estado;
    bool ack = false; // hubo algun ack sobre la falla?
    bool estado_ack = false;
    bool estado_falla = false;
    bool algunoEnFalla = false; // usado para verificar si algun sensor fallo, util para no volver activar actuadores en caso de que otro este en falla
    bool ackExterno = false; //usado para informar ack desde dashboard
    unsigned long lastPeriodStart = 0;
    const int onDuration = 500;
    const int periodDuration = 800;
    LinkedList<Sensor*> sensores = LinkedList<Sensor*>();
    bool envioSMS;
    Menu *menu;
    String numero_cel = "";
    int refresh_dweet = 5000;  //milisegundos
    unsigned long tiempoAnterior = 0; 

   
   public: 
    bool fallaGlobal = false;
    Alarm();
	  void mostrarMenu();
	  bool getEnable();
	  int getMode();
	  void setMode(int);
	  void activar();
	  void desactivar();
	  void verificarSensores();
	  void activarBuzzer();
	  void desactivarBuzzer();
    void activarLED();
    void desactivarLED();
	  void addSensor(Sensor*);
    void enviarEstado();
    void enviarSMS(String msg);
    void procesarAcciones();
    void reset();
    void actualizarDashboard();
    void testEstado();
    void logEvento(String evento, String msg="");
};


class Menu {
protected:
	LiquidCrystal *lcd;
	Keypad *keyPad;
	Alarm *alarma;

	byte flechaAbajoChar[8] = {
		0b00100,
		0b00100,
		0b00100,
		0b00100,
		0b00100,
		0b11111,
		0b01110,
		0b00100
	};

	int level = 0, pos = 1, lcdButton = btnEMPTY, subPos = 1;
	int keyCount = 0;
	bool firstTime = true;
	bool timeOn = false;
	unsigned long prevMillis = 0;
	char CODE[4] = { '1','2','5','6' }; // CODIGO
	char attempt[4] = {0,0,0,0}; 

	int readLCDButtons();
	void ingresaCodigo(int);
	bool verificarCodigo(char *);
	void cambiarModo();

public:
	Menu(Alarm *);
	void printMenu();

};

#endif
