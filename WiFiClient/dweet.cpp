#include "Dweet.h"

//Default constructor, called when a Dweet object is instantiated
Dweet::Dweet(Client& theClient) : client(theClient) {}

//Dweeting method
void Dweet::dweet(String thing, String content) {

  //Establishing connection with dweet.io
  client.connect("www.dweet.io", 80);

  //Sending POST request with json content

 client.print(String("POST ") + "/dweet/quietly/for/" + thing +  " HTTP/1.1\r\n" +
             "Host: dweet.io\r\n" +
             "User-Agent: Arduino/1.0\r\n" +
             "Accept: application/json\r\n" +
             "Content-Type: application/json\r\n" +
             "Accept-Encoding: gzip, deflate, br\r\n" +
             "Accept: application/json\r\n" +
             "Connection: close\r\n" +
             "Content-Length: " + content.length() + "\r\n\r\n" + content 
             );

  client.stop();
}

String Dweet::get_latest(String thing) {
  //Establishing connection with dweet.io
  client.connect("www.dweet.io", 80);
  client.print(String("GET ") + "/get/latest/dweet/for/" + thing +  " HTTP/1.1\r\n" +
             "Host: dweet.io\r\n" +
             "User-Agent: Arduino/1.0\r\n" +
             "Accept: application/json\r\n" +
             "Connection: close\r\n\r\n");

  //Waiting for response
  while(!client.available()) {}
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      break;
    }
  }
  String line = client.readStringUntil('\n');
  //Stopping client
  client.stop();
  return line;
}


