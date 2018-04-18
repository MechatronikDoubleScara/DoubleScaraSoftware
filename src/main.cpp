#include <Arduino.h>
#include <Configuration.h>
#include <Endeffector.h>
#include <SoftwareSerial.h>
#include <ZAxis.h>
#include <Communication.h>
#include <MajorAxis.h>

MajorAxis *Achse;

void setup() {
Serial.begin(19200);
Achse = new MajorAxis();
Achse->setZeroPositionLinks(32,32);
Achse->init();

delay(3000);
Achse->movePosition(50, 150);

delay(3000);
Achse->movePosition(0, -150);


}

void loop() {

Achse->get();
delay(5000);


}
