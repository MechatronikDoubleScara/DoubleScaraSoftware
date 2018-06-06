#ifndef ROBOT
#define ROBOT

#include <math.h>
#include <Endeffector.h>
#include <ZAxis.h>
#include <MajorAxis.h>
#include <ObjectCarrier.h>

class Robot{
public:
  Robot(MajorAxis* MA, ZAxis* ZA, Endeffector* EF) : MajorAxis(MA), ZAxis(ZA), Endeffector(EF) {}

  int movePart(ObjectCarrier* ObjectCarrier1, int x1_pos, int y1_pos, ObjectCarrier* ObjectCarrier2, int x2_pos, int y2_pos, int lockType=0);
  int cleanPlate(ObjectCarrier* ObjectCarrier1, ObjectCarrier* ObjectCarrier2); //clean ObjectCarrier1 and store elements in ObjectCarrier2
  int fillPlate(ObjectCarrier* ObjectCarrier1, ObjectCarrier* ObjectCarrier2); // fill ObjectCarrier2 with suitable Elements of Type of Object Carrier1
private:
  MajorAxis* MajorAxis;
  ZAxis* ZAxis;
  Endeffector* Endeffector;
};


#endif
