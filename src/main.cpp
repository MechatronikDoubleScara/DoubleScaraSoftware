#include <Arduino.h>
#include <Configuration.h>
#include <Endeffector.h>
#include <SoftwareSerial.h>
#include <ZAxis.h>
#include <Communication.h>


ZAxis *Zachse;
Communication *Kommunikation;

void setup() {
Zachse = new ZAxis();
Kommunikation = new Communication();
}

void loop() {

  Kommunikation->recieve();
  //Zachse->movePosition(5);
  //delay(3000);
  Zachse->moveUp();
  //Zachse->moveDown();


  if (Kommunikation->getMode() == 1)
  {
    // CODE OF MODE 1
    Kommunikation->print("#OK, YEAHHH~");
    delay(10);
    Serial.println("I'm in Mode 1");



    Kommunikation->resetRecievedData();
  }
  else if (Kommunikation->getMode() == 2)
  {
    // CODE OF MODE 2
    Kommunikation->print("#YEEAH~");
    delay(10);
    Serial.println("I'm in Mode 2");



    Kommunikation->resetRecievedData();
  }

}
