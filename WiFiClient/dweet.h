#ifndef DWEET_H
#define DWEET_H

#include <Arduino.h>
#include <Client.h>

//Length of static data buffer
#define DATA_BUFFER_LEN 200

class Dweet {
  public:
    Dweet(Client& client);
    //"Dweeting" method
    void dweet(String thing, String data);
    //Getting latest dweet
    String get_latest(String thing);

  private:
    Client& client;

};

#endif //DWEET_H
