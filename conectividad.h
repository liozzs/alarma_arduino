#ifndef conectividad_h
#define conectividad_h

#include "arduino.h"

const int PERIODO_DWEET = 5000;  // 1 minuto

void iniciarComm();
void sendToWIFI(String str);
String leerFromWIFI();
String leerFromSIM900();


#endif

