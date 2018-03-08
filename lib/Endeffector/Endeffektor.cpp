#include <Endeffector.h>

Endeffector::Endeffector(const int Pin = PIN_ENDEFFECTOR): magnetPin(Pin)
{
  pinMode(magnetPin, OUTPUT);
}

void Endeffector::init()
{
  State = false;
  digitalWrite(magnetPin, LOW);
  return;
}

int Endeffector::setState(bool state)
{
  if(state == true)
  {
    digitalWrite(magnetPin, HIGH);
  }
  else if(state == false)
  {
    digitalWrite(magnetPin, LOW);
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
    analogWrite(magnetPin, PwmValue);
  }
  else
  {
    return 1;
  }
  return 0;
}
