#include "Dweet.h"

//Default constructor, called when a Dweet object is instantiated
Dweet::Dweet(Client& theClient) : client(theClient) {}

//Dweeting method
void Dweet::dweet(char* thing, String content) {

  //Establishing connection with dweet.io
  client.connect("dweet.io", 80);

  //Sending POST request with json content
  client.print(String("POST /dweet/quietly/for/") + thing);
  client.println(" HTTP/1.1");
  client.println("Host: dweet.io");
  client.println("User-Agent: Arduino/1.0");
  client.println("Accept: application/json");
  client.println("Connection: close");
  client.println("Content-Type: application/json");
  client.println("Accept-Encoding: gzip, deflate, br");
  
  client.print("Content-Length: ");
  client.println(content.length()+1);
  client.println();
  client.println(content);

  client.stop();
}

char* Dweet::get_latest(char* thing) {
  //Establishing connection with dweet.io
  client.connect("www.dweet.io", 80);

  //Sending GET request
  client.print(F("GET /get/latest/dweet/for/"));
  client.print(thing);
  client.println(F(" HTTP/1.1"));
  client.println(F("Host: dweet.io"));
  client.println(F("User-Agent: Arduino/1.0"));
  client.println(F("Accept: application/json"));
  client.println(F("Connection: close"));
  client.println();

  //Waiting for response
  while(!client.available()) {}

  eatHeader();
  readLine();

  //Stopping client
  client.stop();

  return databuffer;
}

//Consume the header
void Dweet::eatHeader() {
  //Consume lines to the empty line between the end of the header and the beginning of the response body
  while(client.available()) {
    readLine();
    if(strlen(databuffer) == 0) break;
  }
}

//Put incoming data's first line into the data buffer
void Dweet::readLine() {
  //dataptr pointer points to the beginning of the buffer
  dataptr = databuffer;

  char c = client.read();
  while(client.available() && c != '\r') {
    *dataptr++ = c;
    c = client.read();
  }
  client.read();

  *dataptr = 0;
}
