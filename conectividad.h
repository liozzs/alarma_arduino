#ifndef conectividad_h
#define conectividad_h

#include "arduino.h"

void iniciarComm();
void sendToWIFI(String str);
String leerFromWIFI();
void sendToSIM(String str);
String leerFromSIM900();


#endif

