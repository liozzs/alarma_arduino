#include "arduino.h"
#include "alarm.h"
#include "conectividad.h"
#include <ArduinoJson.h>

 //StaticJsonBuffer<300> jsonBuffer;
 
Alarm::Alarm(){
	int eeAddress = 0;

	//Cargar datos guardados
	EEPROM.get(eeAddress, configData);

	if (configData.valid != 100) {

		configData = {
			{ '1','2','5','6' },
			"",
			100
		};

		EEPROM.put(eeAddress, configData);
	}
	else
		numero_cel = configData.tel;

	//SET pin modes
	pinMode(PIN_ENABLE, INPUT);
	pinMode(PIN_ACK, INPUT);

	pinMode(LED, OUTPUT);
	pinMode(BUZZER, OUTPUT);

	this->setMode(MODO_NORMAL);
	this->estado = EST_ACTIVA;

	menu = new Menu(this);

}

void Alarm::mostrarMenu()
{
  ack = !digitalRead(PIN_ACK) || ackExterno;

  menu->printMenu();
}

void Alarm::actualizarClave(char * _clave)
{
	for (int i = 0; i < 4; i++)
		configData.clave[i] = _clave[i];

//	memcpy(&configData.clave, &_clave, sizeof _clave);
	
	EEPROM.put(0, configData);
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

int Alarm::getEstado()
{
	return estado;
}

void Alarm::setMode(int _mode)
{
  this->modoOperacion = _mode;
  switch (_mode) {
    case MODO_NORMAL: {
      Serial.println("MODO ALARMA: NORMAL");
      sendToWIFI("modo:NORMAL");
      break;
    }
    case MODO_TEST: {
      Serial.println("MODO ALARMA: TEST");
       sendToWIFI("modo:TEST");
      break;
    }
    case MODO_MANT: {
      Serial.println("MODO ALARMA: MANTENIMIENTO");
      sendToWIFI("modo:MANTENIMIENTO");
      break;
    }
  }
}

void Alarm::activar()
{
  logEvento("Alarma", "Activando");
  this->estado = EST_ACTIVA;
}

void Alarm::desactivar()
{
  logEvento("Alarma", "Desactivando");
  this->estado = EST_DESACT;
  this->estado_ack = false;
  this->estado_falla = false;
  this->ackExterno = false;
  this->desactivarLED();
  this->desactivarBuzzer();
}

void Alarm::reset(){
  logEvento("Alarma", "RESET");
  desactivar();
  activar();
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

    algunoEnFalla = false;
    //Recorrer lista de sensores
    for(int i = 0; i < sensores.size(); i++) {

      sensor = sensores.get(i); //Obtener sensor de la lista

      falla = sensor->hayFalla();

      if (falla) {
        algunoEnFalla = true;
        fallaGlobal = true;
      }
      
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
    //if (changeAck)        //Actualizar estado ack //comente esto para que no se active la alarma despues del ack, revisar
      //estado_ack = false;
  }

  if (!algunoEnFalla)
    fallaGlobal = false;
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

void Alarm::activarLED(){
  digitalWrite(LED, LOW);
}

void Alarm::desactivarLED(){
  digitalWrite(LED, HIGH);
}

void Alarm::addSensor(Sensor* _sensor)
{
	sensores.add(_sensor);
}

void Alarm::enviarEstado()
{
  StaticJsonBuffer<150> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
 
  Sensor *sensor;
  int lectura;

  //solo se envia la lectura de sensores si esta activada y no es test
  if (this->getEnable() && this->estado != EST_DESACT && this->modoOperacion != MODO_TEST){
    //Recorrer lista de sensores
    for(int i = 0; i < sensores.size(); i++) {
      sensor = sensores.get(i); //Obtener sensor de la lista
      int lectura = sensor->leer();
      bool falla = sensor->hayFalla();
      root[sensor->dweetCode] = String(lectura);
      logEvento("Lectura SENSOR", sensor->nombre + ": " + String(lectura));
    }
  }

  //el estado general se envia siempre.
  root["E"] = String(estado);
  root["A"] = String(estado_ack);
  root["M"] = String(modoOperacion);
  root["EF"] = String(estado_falla);
  
  String output;
  String fullOutput;
  root.printTo(output);
  
  fullOutput = "dweet:frba-ssee-alarma," + output;
  logEvento("Enviado a WIFI", fullOutput);
  sendToWIFI(fullOutput);
}

void Alarm::enviarSMS(String msg){

  if (this->numero_cel == "") {
    logEvento("SIM900",  "NO HAY NUMERO - ingresar via dashboard");
  }
  else {
    logEvento("SIM: Enviando SMS: " + this->numero_cel);
    Serial2.println("AT+CMGF=1\r");
    delay(100); 
    Serial2.println("AT+CMGS=\"+549" + numero_cel + "\""); // send the SMS number
    delay(100); 
    Serial2.println(msg);
    delay(100); 
    Serial2.write(0x1A);
  }
}

Menu::Menu(Alarm *_alarm) {

  //Inicializacion LCD
  lcd = new LiquidCrystal(PIN_LCD_RS, PIN_LCD_EN, PIN_LCD_D0, PIN_LCD_D1, PIN_LCD_D2, PIN_LCD_D3);
  lcd->createChar(1, flechaAbajoChar);
  lcd->begin(16, 2);              
  lcd->setCursor(0, 0);       

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
  case -1:
	  if (lcdButton == btnSELECT) {
		level = 0;
		lcd->blink();
		return;
	  }
	  
	  lcd->noBlink();

	  //Imprime ESTADO
	  lcd->setCursor(0, 0);
	  lcd->print("ESTADO:              ");
	  lcd->setCursor(8, 0);
	  if (alarma->getEstado() == EST_ACTIVA)
		  lcd->print("ACTIVA");
	  else
		  lcd->print("INACTIVA");

	  lcd->setCursor(0, 1);
	  lcd->print("MODO:                ");
	  lcd->setCursor(6,1);

	  //Imprime MODO
	  switch (alarma->getMode()) {
	  case MODO_NORMAL:
		  lcd->print("NORMAL");
		  break;
	  case MODO_TEST:
		  lcd->print("TEST");
		  break;
	  case MODO_MANT:
		  lcd->print("MANT.");
		  break;
	  default:
		  break;
	  }
	  break;

    case 0: //LEVEL 0

      if (lcdButton == btnDOWN && pos < 4)
        pos++;

      if (lcdButton == btnUP && pos > 1)
        pos--;

      if (lcdButton == btnSELECT)
        level = pos;

	  if (lcdButton == btnLEFT)
		  level = -1;

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
        lcd->print(">CAMBIAR CLAVE     ");  //LEVEL 4
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

	case 4: //LEVEL 4 - CAMBIAR CLAVE
	  this->ingresaCodigo(CAMBIO_CLAVE);
	  break;

	case 100: //SUBNIVEL 1 - INGRESA NUEVA CLAVE
	  this->ingresaCodigo(NUEVA_CLAVE);
	  break;
  }

}

LiquidCrystal * Menu::getLCD()
{
	return lcd;
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

	if (_evento == CAMBIO_CLAVE)
		lcd->print("INGRESAR ACTUAL        ");
	else
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

	  if (_evento == NUEVA_CLAVE) {
		  alarma->actualizarClave(attempt);

		  lcd->setCursor(0, 0);
		  lcd->print("->ACTUALIZADA    ");

		  if (millis() - prevMillis >= 2000) {  //Mostramos el mensaje por 2 segundos
			  firstTime = true;
			  timeOn = false;
			  level = 0;              //Vuelve al menu anterior
		  }
		  return;
	  }

      if (_evento != NUEVA_CLAVE && alarma->verificarCodigo(attempt)) {
        lcd->setCursor(0, 0);
		switch (_evento) {
		case ACTIVAR:
			lcd->print("ALARMA -> ACTIVADA     ");
			break;
		case DESACTIVAR:
			lcd->print("ALARMA -> DESACTIVADA  ");
			break;
		case CAMBIO_CLAVE:
			firstTime = true;
			timeOn = false;
			level = 100;		//Nos vamos a un subnivel
			break;
		}

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
        lcd->print("CODIGO INVALIDO     ");
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

bool Alarm::verificarCodigo(char * _attempt)
{
  int correct = 0;

  for (int i = 0; i<4; i++)
  {
    if (_attempt[i] == configData.clave[i])
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

void Alarm::procesarAcciones()
{
  
  //Procesa mensajes de WIFI / Dashboard
  String str = leerFromWIFI();

  logEvento("Recibido de WIFI", str);
  
  if (str != "" && !str.startsWith("debug:", 0)) {
    StaticJsonBuffer<130> jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(str);
    if (!root.success()) {
      logEvento("parseObject() failed");

    }
    logEvento("Ejecutando control de dashboard: ", str);
    if (root.containsKey("estado")){
      if ( root["estado"].as<bool>())
        this->activar();
      else
        this->desactivar();
    }
    if (root.containsKey("reset")) {
        this->desactivar();
        this->activar();
     }
    else if (root.containsKey("ack"))
      this->ackExterno = true;
    else if (root.containsKey("num_cel")) {
      this->numero_cel = root["num_cel"].as<String>();
    }
    else if (root.containsKey("m")) {
      this->setMode(root["m"].as<int>());
    }
    else if (root.containsKey("test_refresh"))
      this->refresh_dweet = root["test_refresh"].as<int>();
    else if (root.containsKey("test_buzzer"))
      if (root["test_buzzer"].as<bool>())
        this->activarBuzzer();
      else
        this->desactivarBuzzer();
    else if (root.containsKey("test_led"))
      if (root["test_led"].as<bool>())
        this->activarLED();
      else
        this->desactivarLED();
    else if (root.containsKey("test_sms"))
        this->enviarSMS(root["test_sms"].as<String>());
    else if (root.containsKey("test_reset_wifi")) {
        logEvento("Reiniciando WIFI");
        sendToWIFI("test:RESET");
    }
    else if (root.containsKey("test_reset_sim")) {
        logEvento("Reiniciando SIM");
        sendToSIM("AT+CFUN=1,1");
        delay(100);
        Serial2.begin(115200);
        iniciarComm();
    }
    else if (root.containsKey("test_conf_portal")) {
        sendToWIFI("test:conf_portal");
    }
 }

  //Procesa mensajes de SIM900 / SMS
  str = leerFromSIM900();
  logEvento("Recibido de SIM", str);
  if (str.indexOf("+CMT:") > 0) {
       if(str.indexOf("ACK") > 0) {
           this->ackExterno = true;
       }else if(str.indexOf("RESET") > 0) {
          this->desactivar();
          this->activar();
       }else if(str.indexOf("DESACTIVAR") > 0) {
          this->desactivar();
       }
   }
}


void Alarm::actualizarDashboard(){
  //Actualizar dashboard cada X seg
  if(millis() > tiempoAnterior + refresh_dweet){ 
    logEvento("Actualizando dashboard");
    this->enviarEstado();
    tiempoAnterior=millis();  
  }
}

void Alarm::testEstado(){

	LiquidCrystal *lcd = menu->getLCD();

  if (this->modoOperacion != MODO_TEST || millis() < prevTimeTestMode + refresh_test)
    return;

  //desactivar salidas
  this->desactivarLED();
  this->desactivarBuzzer();
  
  //delay(4000);
  Serial.println("Iniciando test...");
  lcd->setCursor(0, 0);
  lcd->print("Iniciando test..");
  lcd->setCursor(0, 1);
  lcd->print("                ");
  delay(4000);

  //BUZZER
  Serial.print("Test buzzer...");
  lcd->setCursor(0, 0);
  lcd->print("Test buzzer.....");
  this->activarBuzzer();
  delay(2000);
  this->desactivarBuzzer();
  Serial.println("OK");
  lcd->setCursor(0, 1);
  lcd->print("OK              ");
  delay(1000);
  lcd->setCursor(0, 1);
  lcd->print("                ");

  //LED
  Serial.print("Test LED...");
  lcd->setCursor(0, 0);
  lcd->print("Test LED........");
  this->activarLED();
  delay(2000);
  this->desactivarLED();
  Serial.println("OK");
  lcd->setCursor(0, 1);
  lcd->print("OK              ");
  delay(1000);
  lcd->setCursor(0, 1);
  lcd->print("                ");

  //Sensores
  Sensor *sensor;
  for(int i = 0; i < sensores.size(); i++) {
    sensor = sensores.get(i); //Obtener sensor de la lista
    Serial.print("Test " + sensor->nombre + "...");
	lcd->setCursor(0, 0);
	lcd->print("Test " + sensor->nombre + "............");
	lcd->setCursor(0, 1);
	lcd->print("                ");
	delay(1000);
	if (sensor->leer() != -1) {
		Serial.println("OK");
		lcd->setCursor(0, 1);
		lcd->print("OK              ");
	}

    delay(1000);
  }

  //Limpia valor anterior
  lcd->setCursor(0, 1);
  lcd->print("                ");

  //SIM900
  
  //WIFI
  Serial.print("Test WIFI...");
  lcd->setCursor(0, 0);
  lcd->print("Test WIFI.......");
  //vacio el buffer pendiendte
  while (Serial1.available()) Serial1.read();
  sendToWIFI("test:test_estado");
  delay(2000);
  String msg = leerFromWIFI();
  if (msg.startsWith("debug:WIFI:ack_test:3")){
    Serial.println("OK");
	lcd->setCursor(0, 1);
	lcd->print("OK              ");
  } else {
    Serial.println("ERROR");
	lcd->setCursor(0, 1);
	lcd->print("ERROR           ");
  }

  delay(1000);
  lcd->setCursor(0, 1);
  lcd->print("                ");
  Serial.println("\n\nTest finalizado\n\n");

  lcd->setCursor(0, 0);
  lcd->print("Test finalizado ");
  delay(1000);
  prevTimeTestMode = millis();
}

void Alarm::logEvento(String evento, String msg=""){
//Dependiendo del MODO, uear o no en consola
  if (msg.length() > 2){
    switch (this->modoOperacion) {
      case MODO_NORMAL: {
        break;
      }
      case MODO_TEST: {
        break;
      }
      case MODO_MANT: {
        Serial.println(evento + ": " + msg);
        break;
      }
    }
  }
}

