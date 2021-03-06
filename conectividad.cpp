//Modulo para conectectarse a Dweet y enviar datos de los sensores
//WIFI
#include "conectividad.h"

void iniciarComm(){
  sendToWIFI("test:RESET");
  Serial2.println("AT+CREG=1");  
}

void sendToSIM(String str){
  Serial2.println(str);
}

void sendToWIFI(String str){
  while(str.length() < 128){
    str+='\0';
  }
  Serial1.println(str);
}

String leerFromWIFI(){
  
  String msg_in_wifi = "";
  //Serial.println("INIT: " + String(msg_in_wifi.length()));
  while (Serial1.available() && (msg_in_wifi.length() <= 129)) {
     if (Serial1.available() >0) {
       char c = Serial1.read();
       msg_in_wifi += c;
      }
  }
  // Serial.println("LENGTH:" + String(msg_in_wifi.length()));
   if (msg_in_wifi != "") {
  
      if (msg_in_wifi.startsWith("control:", 0)) {
      //  Serial.println("TIENE CONTROL");
        return msg_in_wifi.substring(8);
      }
      else {
       // Serial.println("SIN CONTROL");
        return "debug:" + msg_in_wifi;
      }
   }
   return "";
}

String leerFromSIM900(){
   String msg_in_sim = "";
  
  while (Serial2.available()) {
     if (Serial2.available()) {
       char c = Serial2.read();
       msg_in_sim += c;
     }
  }
  
  return msg_in_sim;

}

