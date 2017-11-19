//Modulo para conectectarse a Dweet y enviar datos de los sensores
//WIFI
#include "conectividad.h"

void sendToWIFI(String str){
  while(str.length() < 128){
    str+='\0';
  }
  Serial1.println(str);
}

String leerFromWIFI(){
    
  String msg_in_wifi = "";
  
  while (Serial1.available()) {
     if (Serial1.available() >0) {
       char c = Serial1.read();
       msg_in_wifi += c;
      }
  }

   if (msg_in_wifi.length() >= 128) {

      if (msg_in_wifi.startsWith("control:", 0)) {
        return msg_in_wifi.substring(8);
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
   Serial.println(msg_in_sim);
  if (msg_in_sim.startsWith("+CMT", 0)) {
          Serial.println(msg_in_sim.substring(4));
   }
   return "";

}

