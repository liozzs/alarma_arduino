#ifndef config_pin_h
#define config_pin_h

//CONFIGURACION GLOBAL
const int MAIN_LOOP_DELAY = 100;
const int DELAY_POST = 2000; // 60 minuto
const int DELAY_GET = 10000; //10 segundos

const int WIFI_ENABLE = true;

//CONFIGURACION PINES
const int PIN_ENABLE = 22;
const int PIN_ACK = 31;

//
const int SENSOR_LLAMA = A0;
const int SENSOR_TEMPERATURA = 2;


//ACTUADORES
const int LED = 22;
const int BUZZER = 24;

//TECLADO
const byte TECLADO_ROWS = 4; //filas
const byte TECLADO_COLS = 4; //columnas

const byte rowPins[TECLADO_ROWS] = {9,8,7,6}; 
const byte colPins[TECLADO_COLS] = {5,4,3,2};

const char hexaKeys[TECLADO_ROWS][TECLADO_COLS] = {
  {'1','2','3', 'A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

#endif

