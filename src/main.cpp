#include <Arduino.h>
#include <Configuration.h>
#include <Endeffector.h>
#include <SoftwareSerial.h>
#include <ZAxis.h>
#include <Communication.h>
#include <MajorAxis.h>


double lastX = 0;
double lastY = 0;
double lastZ = 0;
int lastMagnet = 0;


ZAxis *zAchse;
Endeffector *Magnet;
MajorAxis *Achsen;
Communication *Kommunikation;

void setup() {
Serial.begin(9600);
Serial1.begin(9600);
Kommunikation = new Communication();
zAchse = new ZAxis();
Magnet = new Endeffector();
Achsen = new MajorAxis();


//zAchse->moveDown();

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
    //delay(3000);
    Serial.println("Send OK\n");
    Kommunikation->print("#OK~");
    Kommunikation->resetRecievedData();
  }
  else if (Kommunikation->getMode() == 1)
  {
    // CODE OF MODE 1
    Serial.println("I'm in Mode 1: Manual Mode");

    int success = 0;

    if((success = (Achsen->calculateAngles(Kommunikation->getCoordinate('X'), Kommunikation->getCoordinate('Y')))) == 1)
    {
      Achsen->printAngles();

      if(lastX != Kommunikation->getCoordinate('X') || lastY != Kommunikation->getCoordinate('Y'))
      {
        zAchse->moveUp();
        delay(200);
        Serial.println("Make XY Move");
        Achsen->movePosition(Kommunikation->getCoordinate('X'),Kommunikation->getCoordinate('Y'));
      }
      delay(200);
      if(lastZ != Kommunikation->getCoordinate('Z'))
      {
        Serial.println("Make Z Move");
        zAchse->movePosition(Kommunikation->getCoordinate('Z'));
      }
      delay(200);
      if(lastMagnet != Kommunikation->getMagnet())
      {
        Serial.println("Make Magnet Change");
        Magnet->setState(Kommunikation->getMagnet());
      }

      // ACCESS ANGLES with getter func -> Hauptachse->getAngle1() und Hauptachse->getAngle4()

      lastX = Kommunikation->getCoordinate('X');
      lastY = Kommunikation->getCoordinate('Y');
      lastZ = Kommunikation->getCoordinate('Z');
      lastMagnet = Kommunikation->getMagnet();

      Serial.println("Send OK\n");
      Kommunikation->print("#OK~");
      Kommunikation->resetRecievedData();
    }
    else if(success == -1)
    {
      // Cordinate not in workspace
      delay(10);
      Serial.println("Send Error 1\n");
      Kommunikation->print("#E1~");
      Kommunikation->resetRecievedData();
    }
    else if(success == -2)
    {
      // Cordinate not in workspace
      delay(10);
      Serial.println("Send Error 2\n");
      Kommunikation->print("#E2~");
      Kommunikation->resetRecievedData();
    }
    else if(success == -3)
    {
      // Cordinate not in workspace
      delay(10);
      Serial.println("Send Error 3\n");
      Kommunikation->print("#E3~");
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


//Achsen->printSensorValue();
//delay(500);

}
