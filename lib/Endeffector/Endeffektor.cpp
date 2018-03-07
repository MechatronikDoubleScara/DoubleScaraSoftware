#include <Endeffector.h>

Endeffector::Endeffector()
{
  pinMode(Pin, OUTPUT);
}

void Endeffector::init()
{
  State = false;
  digitalWrite(Pin, LOW);
  return;
}

int Endeffector::setState(bool state)
{
  if(state == true)
  {
    digitalWrite(Pin, HIGH);
  }
  else if(state == false)
  {
    digitalWrite(Pin, LOW);
  }
  else
  {
    return 1;
  }
  return 0;
}

int Endeffector::setPWM(int PwmValue)
{
  if((PwmValue > 0) || (PwmValue < 255))
  {
    analogWrite(Pin, PwmValue);
  }
  else
  {
    return 1;
  }
  return 0;
}
