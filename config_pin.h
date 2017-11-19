#ifndef _CONFIG_PIN_h
#define _CONFIG_PIN_h

//CONFIGURACION GLOBAL
const int MAIN_LOOP_DELAY = 100;

//CONFIGURACION PINES ALARMA
const int PIN_ENABLE = 22;
const int PIN_ACK = 31;

//SENSORES
const int PIN_SENSOR_LLAMA = A0;
const int PIN_SENSOR_TEMPERATURA = 28; //lee temperatura y humedad al mismo tiempo
const int PIN_SENSOR_PIR = 33;
const int PIN_SENSOR_CO2 = A1;

//LCD
const int PIN_LCD_KEYPAD = A1;
const int PIN_LCD_RS = 8;
const int PIN_LCD_EN = 9;
const int PIN_LCD_D0 = 4;
const int PIN_LCD_D1 = 5;
const int PIN_LCD_D2 = 6;
const int PIN_LCD_D3 = 7;

//ACTUADORES
const int LED = 26;
const int BUZZER = 24;

//TECLADO
const byte TECLADO_ROWS = 4; //filas
const byte TECLADO_COLS = 3; //columnas

const byte rowPins[TECLADO_ROWS] = {39,41,43,45}; 
const byte colPins[TECLADO_COLS] = {47,49,51};

const char hexaKeys[TECLADO_ROWS][TECLADO_COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

#endif
