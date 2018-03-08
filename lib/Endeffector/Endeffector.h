#ifndef ENDEFFECTOR
#define ENDEFFECTOR

#include <Configuration.h>
#include <Arduino.h>

class Endeffector
{
  public:
    Endeffector(const int Pin = PIN_ENDEFFECTOR);
    void init();
    int setState(bool state);
    int setPWM(int PwmValue);

  private:
    const int magnetPin;
    bool State = false;
    int PwmValue = 0;
};

#endif
