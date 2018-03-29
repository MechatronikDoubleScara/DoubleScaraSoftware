#ifndef MAJORAXIS
#define MAJORAXIS

#include <Configuration.h>
#include <Arduino.h>
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
    void moveLinksSingleStep(int steps1, int steps2);
    void moveLinksMicroStep(int steps1, int steps2);

  private:
    AS5048A *angleSensor1;
    AS5048A *angleSensor2;
    float sensor_offset1, sensor_offset2;
    float stepper_max_speed, stepper_acceleration;

    AccelStepper *stepper1_Single;
    AccelStepper *stepper2_Single;
    AccelStepper *stepper1_Micro;
    AccelStepper *stepper2_Micro;
};

#endif
