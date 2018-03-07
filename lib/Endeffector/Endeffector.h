#ifndef ENDEFFECTOR
#define ENDEFFECTOR

#include <Configuration.h>
#include <Arduino.h>

class Endeffector
{
  public:
    Endeffector();
    void init();
    int setState(bool state);
    int setPWM(int PwmValue);

  private:
    int Pin = PIN_Magnet;
    bool State = false;
    int PwmValue = 0;
};

#endif
