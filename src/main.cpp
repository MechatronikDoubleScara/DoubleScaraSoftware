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
Achse->setZeroPositionLinks(29,0);
Achse->init();

}

void loop() {


}
