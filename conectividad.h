#ifndef conectividad_h
#define conectividad_h

//WIFI
#include "WiFiEsp.h"

const char ssid[] = "Fibertel WiFi144";   // your network SSID (name)
const char pass[] = "0043240426";        // your network password

const unsigned long lastConnectionTime = 0;         // last time you connected to the server, in milliseconds
const unsigned long postingInterval = 10000L; // delay between updates, in milliseconds

WiFiEspClient getClienteWiFi();
void printWifiStatus();


#endif

