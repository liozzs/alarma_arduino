#include "Arduino.h"
#include "Alarm.h"
#include "config_pin.h"

Alarm::Alarm(Sensor* sensor) {
   sensor = sensor;
   
   //lcd = lcd;
};

void Alarm::check(bool enable, bool falla, bool ack, String mode) {
    String modo_alarma = mode;

    if (ack == true) {
      estado_ack = true;
      estado_falla = false;
    }
    if (enable) {
          if (falla && !estado_ack) 
            this->executeNormal();
          else if (falla && estado_ack)
            this->executeAck();
          else if (!falla)
            if (estado_falla) 
              this->executeBlink();
            else {
              estado_ack = false;
              this->executeDesactivar();
            }
        }
        //guardo estado previo de falla
        if (falla == true && !estado_ack)
          estado_falla = true;

};


AlarmaLlama::AlarmaLlama(Sensor* sensor) : Alarm(sensor){


}

void AlarmaLlama::executeNormal() {
     Serial.println("execute normal");
     digitalWrite(LED, HIGH);

     if (millis()-lastPeriodStart>=periodDuration)
  {
    lastPeriodStart+=periodDuration;
    tone(BUZZER, 800, onDuration); // play 800 Hz tone in background for 'onDuration'
  }
}
void AlarmaLlama::executeAck() {
     Serial.println("execute ack");
     noTone(BUZZER);
}
void AlarmaLlama::executeBlink() {
     Serial.println("execute blink");
     digitalWrite(LED, millis()>>9 &1); // blinking sin usar delay
}
void AlarmaLlama::executeDesactivar() {
     Serial.println("execute desactivar");
     digitalWrite(LED, LOW);
     noTone(BUZZER);
}

void AlarmaLlama::check(bool enable, bool fail, bool ack, String mode) {
     Alarm::check(enable,fail, ack,  mode);
}



