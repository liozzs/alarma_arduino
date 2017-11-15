#include "arduino.h"
#include "alarm.h"
#include <ArduinoJson.h>

 StaticJsonBuffer<300> jsonBuffer;
 
Alarm::Alarm(){
	   
	//SET pin modes
	pinMode(PIN_ENABLE, INPUT);
	pinMode(PIN_ACK, INPUT);

	pinMode(LED, OUTPUT);
	pinMode(BUZZER, OUTPUT);

	this->modoOperacion = MODO_NORMAL;
  this->estado = EST_ACTIVA;

  menu = new Menu(this);

}

void Alarm::mostrarMenu()
{
  ack = !digitalRead(PIN_ACK);

  menu->printMenu();
}

bool Alarm::getEnable()
{
	int enable = digitalRead(PIN_ENABLE);
	
	if (enable == HIGH)
		return true;
	else
		return true;
}

int Alarm::getMode()
{
	return modoOperacion;
}

void Alarm::setMode(int _mode)
{
  this->modoOperacion = _mode;
}

void Alarm::activar()
{
  this->estado = EST_ACTIVA;
}

void Alarm::desactivar()
{
  this->estado = EST_DESACT;
  this->estado_ack = false;
  this->estado_falla = false;
}

void Alarm::verificarSensores()
{
  Sensor *sensor;
  bool falla;
  bool changeAck = false;

  // guardo cuando se hizo ack
  if (ack == true) {
    estado_ack = true;
    estado_falla = false;
    ack = false;
  }
  
  changeAck = !estado_ack;

  if (this->getEnable() && this->estado != EST_DESACT){

    //Recorrer lista de sensores
    for(int i = 0; i < sensores.size(); i++) {

      sensor = sensores.get(i); //Obtener sensor de la lista

      falla = sensor->hayFalla();

      if (falla && !estado_ack)
        sensor->executeNormal();
      else if (falla && estado_ack)
        sensor->executeAck();
      else if (!falla) {
        if (sensor->estado_falla)
          sensor->executeBlink();
        else {
          changeAck = true;
          sensor->executeDesactivar();
        }
      }
      //guardo estado previo de falla
      if (falla == true && changeAck){
        sensor->estado_falla = true;
        estado_falla = true;
      }
    }
    if (changeAck)        //Actualizar estado ack
      estado_ack = false;
  }
}

void Alarm::activarBuzzer()
{
	if (millis() - lastPeriodStart >= periodDuration)
	{
		lastPeriodStart += periodDuration;
		tone(BUZZER, 800, onDuration); // play 800 Hz tone in background for 'onDuration'
	}
}

void Alarm::desactivarBuzzer()
{
	noTone(BUZZER);
}

void Alarm::addSensor(Sensor* _sensor)
{
	sensores.add(_sensor);
}

JsonObject& root = jsonBuffer.createObject();

void sendToWIFI(String str){
  while(str.length() < 128){
    str+='\0';
  }
  Serial1.println(str);
}

void Alarm::enviarEstado()
{
  Sensor *sensor;

  int lectura;
  
  
  //Recorrer lista de sensores
  for(int i = 0; i < sensores.size(); i++) {
    sensor = sensores.get(i); //Obtener sensor de la lista
    float lectura = sensor->leer();
    bool falla = sensor->hayFalla();

    root[sensor->nombre] = String(lectura);
    //root["falla" + sensor->nombre] = String(falla);
  }
  //root["Estado"] = String(estado);
  //root["Modo"] = String(modoOperacion);
  
  String output;
  String fullOutput;
  root.printTo(output);

  //Serial1 es donde se conecta el WIFI
  //sendToWIFI("modo:TEST");
  delay(100);

  fullOutput = "dweet:frba-ssee-alarma," + output;
  //Serial1.write(fullOutput);

  sendToWIFI(fullOutput);
}


Menu::Menu(Alarm *_alarm) {

  //Inicializacion LCD
  lcd = new LiquidCrystal(PIN_LCD_RS, PIN_LCD_EN, PIN_LCD_D0, PIN_LCD_D1, PIN_LCD_D2, PIN_LCD_D3);
  lcd->createChar(1, flechaAbajoChar);
  lcd->begin(16, 2);              
  lcd->setCursor(0, 0);       
  lcd->blink();

  //Inicializacion KEY PAD
  keyPad = new Keypad(makeKeymap(hexaKeys), rowPins, colPins, TECLADO_ROWS, TECLADO_COLS);

  alarma = _alarm;
}

int Menu::readLCDButtons()
{
  int adc_key_in = 0;

  adc_key_in = analogRead(PIN_LCD_KEYPAD);

  if (adc_key_in > 1000) return btnNONE;

  // For V1.0 us this threshold
  if (adc_key_in < 50)   return btnRIGHT;
  if (adc_key_in < 195)  return btnUP;
  if (adc_key_in < 380)  return btnDOWN;
  if (adc_key_in < 555)  return btnLEFT;
  if (adc_key_in < 790)  return btnSELECT;

  return btnEMPTY;    //No se presiono ningun boton
}

void Menu::printMenu() {

  lcdButton = this->readLCDButtons();

  switch (level) {

    case 0: //LEVEL 0
      if (lcdButton == btnDOWN && pos < 4)
        pos++;

      if (lcdButton == btnUP && pos > 1)
        pos--;

      if (lcdButton == btnSELECT)
        level = pos;

      if (pos == 1 || pos == 2) {
        lcd->setCursor(0, 0);
        lcd->print(">ACTIVAR           ");  //LEVEL 1
        lcd->setCursor(0, 1);
        lcd->print(">DESACTIVAR  ..\001");  //LEVEL 2
        lcd->setCursor(0, pos - 1);
      }

      if (pos == 3 || pos == 4) {
        lcd->setCursor(0, 0);
        lcd->print(">CAMBIAR MODO      ");  //LEVEL 3
        lcd->setCursor(0, 1);
        lcd->print(">AGREGAR SENSOR    ");  //LEVEL 4
        lcd->setCursor(0, pos - 3);
      }
      break;

    case 1: //LEVEL 1 - ACTIVAR
      this->ingresaCodigo(ACTIVAR);
      break;

    case 2: //LEVEL 2 - DESACTIVAR
      this->ingresaCodigo(DESACTIVAR);
      break;

    case 3: //LEVEL 3 - CAMBIAR MODO
      this->cambiarModo();
      break;
  }

}

void Menu::ingresaCodigo(int _evento)
{
  if (lcdButton == btnLEFT) {
    level = 0;
    firstTime = true;
    return;
  }

  if (firstTime) {
    lcd->setCursor(0, 0);
    lcd->print("INGRESAR CODIGO        ");
    lcd->setCursor(0, 1);
    lcd->print(">                      ");
    lcd->setCursor(1, 1);
    keyCount = 0;
    firstTime = false;
  }
  else {
    char key = keyPad->getKey();

    if (key != NO_KEY)
    {
      attempt[keyCount] = key;
      keyCount++;

      lcd->setCursor(1 + keyCount, 1);
      lcd->print(key);
    }

    if (keyCount == sizeof(attempt)) {
      if (!timeOn) {
        prevMillis = millis();
        timeOn = true;
      }

      if (this->verificarCodigo(attempt)) {
        lcd->setCursor(0, 0);
        if(_evento == ACTIVAR)
          lcd->print("ALARMA -> ACTIVADA     ");
        else
          lcd->print("ALARMA -> DESACTIVADA  ");

        lcd->setCursor(0, 1);
        lcd->print("                       ");

        if (millis() - prevMillis >= 2000) {  //Mostramos el mensaje por 2 segundos
          if (_evento == ACTIVAR)
            alarma->activar();          //Activamos la alarma
          else
            alarma->desactivar();       //Desactivamos la alarma

          firstTime = true;
          timeOn = false;
          level = 0;              //Vuelve al menu anterior
        }
      }
      else {
        lcd->setCursor(0, 0);
        lcd->print("CODIGO  INCORRECTO     ");
        lcd->setCursor(0, 1);
        lcd->print("                       ");

        if (millis() - prevMillis >= 2000) {  //Mostramos el mensaje por 2 segundos
          firstTime = true;         //Resetea el LCD para que se ingrese la clave nuevamente
          timeOn = false;
        }
      }
    }

  }
}

bool Menu::verificarCodigo(char * _attempt)
{
  int correct = 0;

  for (int i = 0; i<4; i++)
  {
    if (_attempt[i] == CODE[i])
      correct++;
  }

  if (correct == 4)
    return true;
  else
    return false;
}

void Menu::cambiarModo()
{
  if (lcdButton == btnLEFT) {
    level = 0;
    subPos = 1;
    return;
  }

  if (lcdButton == btnDOWN && subPos < 3)
    subPos++;

  if (lcdButton == btnUP && subPos > 1)
    subPos--;

  if (lcdButton == btnSELECT) {
    prevMillis = millis();
    timeOn = true;
  }

  if (!timeOn) {
    if (subPos == 1 || subPos == 2) {
      lcd->setCursor(0, 0);
      lcd->print(">MODO NORMAL    "); //MODO 1
      lcd->setCursor(0, 1);
      lcd->print(">MODO TEST   ..\001");  //MODO 2
      lcd->setCursor(0, subPos - 1);
    }

    if (subPos == 3) {
      lcd->setCursor(0, 0);
      lcd->print(">MODO MANT.     "); //MODO 3
      lcd->setCursor(0, 1);
      lcd->print("                ");
      lcd->setCursor(0, subPos - 3);
    }
  }
  else {
    lcd->setCursor(0, 0);
    lcd->print("MODO ACTUALIZADO   ");
    lcd->setCursor(0, 1);
    lcd->print("                   ");

    if (millis() - prevMillis >= 2000) {  //Mostramos el mensaje por 2 segundos
      alarma->setMode(subPos);      //Setear Modo de operacion
      subPos = 1;
      timeOn = false;
      level = 0;              //Vuelve al menu anterior
    }
  }
}
