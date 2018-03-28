#include <Arduino.h>
#include <Configuration.h>
#include <Endeffector.h>
#include <SoftwareSerial.h>
#include <ZAxis.h>
#include <Communication.h>
#include <MajorAxis.h>


ZAxis *Zachse;
Communication *Kommunikation;
MajorAxis *Hauptachse;

void setup() {
Zachse = new ZAxis();
Kommunikation = new Communication();
Hauptachse = new MajorAxis();
}

void loop() {

  Kommunikation->recieve();
  //Zachse->movePosition(5);
  //delay(3000);
  //Zachse->moveUp();
  //Zachse->moveDown();

  if (Kommunikation->getMode() == 0)
  {
    // CODE OF MODE 1
    Serial.println("I'm in Mode 0: Clear Plate");
    switch(Kommunikation->getActivity())
    {
      case 1:
        // In Activity 1 do nothing because ManualMode
        Serial.println("In Activity 1 do nothing because ManualMode");
        break;
      case 2:
        // In Activity 2 clear Build Plate
        Serial.println("In Activity 2 clear Build Plate");
        break;
      case 3:
        // In Activity 3 clear Build Plate
        Serial.println("In Activity 3 clear Build Plate");
        break;
      default:
        Serial.println("error no activity transmitted");
        break;
    }
    delay(3000);
    Serial.println("Send OK\n");
    Kommunikation->print("#OK~");
    Kommunikation->resetRecievedData();
  }
  else if (Kommunikation->getMode() == 1)
  {
    // CODE OF MODE 1
    Serial.println("I'm in Mode 1: Manual Mode");

    if((Hauptachse->calculateAngles(Kommunikation->getCoordinate('X'), Kommunikation->getCoordinate('Y'))) == 1)
    {
      Hauptachse->printAngles();
      // GEORGS CODE für Motorbewegung
      // ACCESS ANGLES with getter func -> Hauptachse->getAngle1() und Hauptachse->getAngle4()
      delay(3000);
      Serial.println("Send OK\n");
      Kommunikation->print("#OK~");
      Kommunikation->resetRecievedData();
    }
    else
    {
      // Cordinate not in workspace
      delay(3000);
      Serial.println("Send OK\n");
      Kommunikation->print("#OK~");
      Kommunikation->resetRecievedData();
    }


  }
  else if (Kommunikation->getMode() == 2)
  {
    // CODE OF MODE 2
    Serial.println("I'm in Mode 2: Shapes");
    delay(3000);
    Serial.println("Send OK\n");
    Kommunikation->print("#OK~");
    Kommunikation->resetRecievedData();
  }
  else if (Kommunikation->getMode() == 3)
  {
    // CODE OF MODE 2
    Serial.println("I'm in Mode 3: TicTacToe");
    delay(3000);
    Serial.println("Send OK\n");
    Kommunikation->print("#OK~");
    Kommunikation->resetRecievedData();
  }


}
