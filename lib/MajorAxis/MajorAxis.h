#ifndef MAJORAXIS
#define MAJORAXIS

#include <Configuration.h>
#include <Arduino.h>
#include <math.h>
#include <AS5048A.h>
#include <Wire.h>
#include <AccelStepper.h>
#include <Adafruit_MotorShield.h>

class MajorAxis
{
  public:
    MajorAxis();
    void init();
    void setZeroPositionLinks(float offset1, float offset2);
    void setMomvmentParameter(int speed, int maxspeed, int acc);
    int movePosition(float X, float Y);
    void moveToAngle(double alpha, double beta);
    void printSensorValue();

    int calculateAngles(double X, double Y); // returns 1 if position is inside work space, -1 if outside workspace, -2 if position is in safety area
    void printAngles();
    double getAngle1();
    double getAngle4();

  private:
    double calculateToGoAngle(double targetAngle, int motoridx);
    void moveLinksStep(int steps1, int steps2);
    void changeSide();

    AS5048A *angleSensor1;
    AS5048A *angleSensor2;
    float sensor_offset1, sensor_offset2;
    float stepper_max_speed, stepper_acceleration;

    AccelStepper *stepper1;
    AccelStepper *stepper2;

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
    int currentArea;

    double currentPosX;
    double currentPosY;

    // Variables for safety check
    double motorSafetyDistance;
    double ArmDistanceLeft;
    double ArmDistanceRight;
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
