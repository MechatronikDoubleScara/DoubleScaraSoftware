#ifndef COMMUNICATION
#define COMMUNICATION

#include <SoftwareSerial.h>
#include <Configuration.h>
#include <Arduino.h>


class Communication
{
public:
  Communication():bluetooth(PIN_SOFTWARESERIAL_RX,PIN_SOFTWARESERIAL_TX)
  {
    init();
  }
  void init();
  void recieve();
  void print(String message);
  void resetRecievedData();

  int getMode();
  double getCoordinate(char axis);


private:
  SoftwareSerial bluetooth;
  char command;
  String recievedData; // contains the whole string of recieved data from bluetooth connection
  String temp; // contains the temporary sub messages that were split up
  String data[MAX_STRING_ENTRIES] = {}; // contains the split data messages
  int mode;
  // Variables for mode 1
  double X;
  double Y;
  double Z;
  int magnet;
  // Variables for mode 2

  // Variables for mode 3


};



#endif
