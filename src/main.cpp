#include <Arduino.h>
#include <Endeffector.h>
#include <Configuration.h>

Endeffector *Magnet;

void setup() {
Magnet = new Endeffector();
}

void loop() {
  
  //Test of magnet mit On/Off and PWM
  /*Magnet->setPWM(50);
  delay(3000);
  Magnet->setState(true);
  delay(5000);
  Magnet->setPWM(50);
  delay(3000);
  Magnet->setState(false);
  delay(5000);*/

}
