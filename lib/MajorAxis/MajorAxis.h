#ifndef MAJORAXIS
#define MAJORAXIS

#include <Configuration.h>
#include <Arduino.h>
#include <math.h>

class MajorAxis
{
public:
  MajorAxis()
  {
    init();
  }
  void init();
  int calculateAngles(double X, double Y); // returns 1 if position is inside work space, -1 if outside workspace, -2 if position is in safety area
  void printAngles();
  double getAngle1();
  double getAngle4();


private:
  double LengthLeft;
  double LengthRight;

  double alpha1;
  double alpha2;
  double PHI1;
  double PHI1d;

  double beta1;
  double beta2;
  double PHI4;
  double PHI4d;

  int area;

  // Variables for safety check
  double DSafetyDistance;
  double leftJoint;
  double rightJoint;
  double SafetyDistanceTCP;
  double distanceTCP1;
  double distanceTCP2;
  double distanceTCP3;
  double distanceTCP4;

  double Ax;
  double Ay;
  double Bx;
  double By;
  double D;

};


#endif
