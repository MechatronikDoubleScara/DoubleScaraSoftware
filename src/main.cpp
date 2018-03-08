#include <Arduino.h>
#include <Configuration.h>
#include <Endeffector.h>
#include <zAxis.h>

ZAxis *Zachse;

void setup() {
Zachse = new ZAxis();
}

void loop() {

  //Zachse->movePosition(5);
  //delay(3000);
  Zachse->moveUp();
  //Zachse->moveDown();


}
