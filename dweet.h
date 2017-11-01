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
    void dweet(char* thing, String data);
    //Getting latest dweet
    char* get_latest(char* thing);

  private:
    Client& client;
    //Incoming data reading attributes
    char databuffer[DATA_BUFFER_LEN];
    char* dataptr;

    //Private incoming data reading methods
    void eatHeader();
    void readLine();
};

#endif //DWEET_H
