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
  int calculateAngles(double X, double Y);
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
};


#endif
