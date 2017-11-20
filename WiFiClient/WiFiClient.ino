#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino

//needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager

//custom imports
#include "Dweet.h" 
#include <ArduinoJson.h>

String msg_in;
String msg_out;
DynamicJsonBuffer  jsonBuffer;
//para medir el tiempo y ejecutar GET y POST 
long previousMillis = 0;
String prev_get = "";
String modo_operacion = "NORMAL";

WiFiClient client;
Dweet dweet(client);

void setup() {
  Serial.begin(115200);

  
  //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;
  wifiManager.setTimeout(180);
  
  if(!wifiManager.autoConnect("AutoConnectAP")) {
    Serial.println("failed to connect and hit timeout");
    delay(3000);
    //reset and try again, or maybe put it to deep sleep
    ESP.reset();
    delay(5000);
  } 
  client.setNoDelay(true);
  //if you get here you have connected to the WiFi
  Serial.println("WIFI: connected...:)");
}

void loop() {
  
   while (Serial.available()) {
     if (Serial.available() > 0) {
       char c = Serial.read();
       msg_in += c;
     }
   }
   debug("WIFI: msg de arduino length: " + String(msg_in.length()));
   if (msg_in.length() >= 128) {
     if (msg_in.indexOf("modo:TEST") > 0) {
        modo_operacion = "TEST";
        debug("WIFI: cambio modo a TEST");
     } else if (msg_in.indexOf("modo:MANTENIMIENTO") > 0) {
        modo_operacion = "MANTENIMIENTO";
        debug("WIFI: cambio modo a MANTENIMIENTO");
     } else if (msg_in.indexOf("modo:NORMAL") > 0) {
        modo_operacion = "NORMAL";
        debug("WIFI: cambio modo a NORMAL");
     }

     if (msg_in.indexOf("test:estado_WIFI") > 0) {    
        if (WiFi.status() != WL_CONNECTED){
           debug("WIFI ERROR");
        } else {
           debug("WIFI OK");
        }
     }

     if (msg_in.indexOf("test:RESET") > 0) {    
        debug("WIFI: reiniciando");
        ESP.reset();
        delay(5000);
        return;
     }

     if (msg_in.startsWith("dweet:", 0)) {
        String thing, contents;
        for (int i = 0; i < msg_in.length(); i++) {
          if (msg_in.substring(i, i+1) == ",") {
            thing = msg_in.substring(6, i);
            contents = msg_in.substring(i+1);
            break;
          }
        }    
        debug("WIFI: procesando dweet post: " + thing + " " + contents);
        dweet.dweet(thing, contents);
        msg_in = "";
     }
     msg_in = "";
   }
   
      msg_out = dweet.get_latest("frba-ssee-control");
      debug("leido: " + msg_out);
      JsonObject& root = jsonBuffer.parseObject(msg_out);
       if (!root.success()) {
        debug("parseObject() failed");
        return;
      }
      String control = "control:" + root["with"][0]["content"].as<String>();
      jsonBuffer.clear();
      if (msg_out != prev_get) {
        prev_get = msg_out;
        sendToArduino(control); //envia al arduino la lectura de dweet
        debug("WIFI: dweet get: " + msg_out);
      } else {
        debug("WIFI: no hay comando nuevo");
      }

    if (WiFi.status() != WL_CONNECTED){
        debug("WIFI DESCONECTADO");
        ESP.reset();
        delay(5000);
    }

}

void sendToArduino(String str){
  while(str.length() < 128){
    str+='\0';
  }
  Serial.println(str);
}

void debug(String str) {
  if (modo_operacion == "TEST") 
     sendToArduino("test:" + str);
  else if (modo_operacion == "MANTENIMIENTO") 
     sendToArduino(str);
  else {

  }
}

