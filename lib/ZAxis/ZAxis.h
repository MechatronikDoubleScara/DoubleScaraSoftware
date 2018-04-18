#ifndef ZAXIS
#define ZAXIS

#include <Configuration.h>
#include <Arduino.h>
#include <VarSpeedServo.h>

class ZAxis
{
  public:
    ZAxis(const int Pin = PIN_ZAXIS_SERVO, int speed = Z_SERVO_SPEED);
    //void init();
    int movePosition(float position);
    int moveUp();
    int moveDown();
    void setSpeed(int speed);
    void setPosUp(float pos);
    void setPosDown(float pos);

  private:
    VarSpeedServo zServo;
    const int servoPin;
    float currentPos;
    int zSpeed;
    float posUp;
    float posDown;
    //const int limitUp = ZAXIS_LIMIT_UP;
    //const int limitDown = ZAXIS_LIMIT_DOWN;
};

#endif
