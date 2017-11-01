//Modulo para conectectarse a Dweet y enviar datos de los sensores
//WIFI
#include "conectividad.h"

WiFiEspClient getClienteWiFi() {
  	WiFiEspClient client;
    int status = WL_IDLE_STATUS;     // the Wifi radio's status

	  // initialize serial for ESP module
	  Serial1.begin(9600);
	  // initialize ESP module
	  WiFi.init(&Serial1);

	  // check for the presence of the shield
	  if (WiFi.status() == WL_NO_SHIELD) {
	    Serial.println("WiFi shield not present");
	    // don't continue
	    while (true);
	  }

	  // attempt to connect to WiFi network
	  while ( status != WL_CONNECTED) {
	    Serial.print("Attempting to connect to WPA SSID: ");
	    Serial.println(ssid);
	    // Connect to WPA/WPA2 network
	    status = WiFi.begin(ssid, pass);
	  }

	  Serial.println("You're connected to the network");
	  
	  printWifiStatus();
}

void printWifiStatus()
{
  // print the SSID of the network you're attached to
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength
  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

//ETHERNET

