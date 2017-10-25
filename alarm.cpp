#include "Arduino.h"
#include "Alarm.h"


Alarm::Alarm(Sensor* sensor) {
   sensor = sensor;
   //lcd = lcd;
};

void Alarm::check(bool enable, bool falla, bool ack, String mode) {
    String modo_alarma = mode;

    if (enable) {

      if (falla && !ack) 
            this->executeNormal();
          else if (falla && ack)
            this->executeAck();
          else if (!falla)
            if (estado_falla) 
              this->executeBlink();
            else
              this->executeDesactivar();
        }
        //guardo estado previo de falla
      estado_falla = falla;

};

AlarmaLlama::AlarmaLlama(Sensor* sensor) : Alarm(sensor){
  
}

void AlarmaLlama::executeNormal() {
     Serial.println("execute normal");
}
void AlarmaLlama::executeAck() {
     Serial.println("execute ack");
}
void AlarmaLlama::executeBlink() {
     Serial.println("execute blink");
}
void AlarmaLlama::executeDesactivar() {
     Serial.println("execute desactivar");
}

void AlarmaLlama::check(bool enable, bool fail, bool ack, String mode) {
     Alarm::check(enable,fail, ack,  mode);
}



