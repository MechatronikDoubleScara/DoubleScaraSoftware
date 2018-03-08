#include <ZAxis.h>

ZAxis::ZAxis(const int Pin = PIN_ZAXIS_SERVO, int speed = 50): servoPin(Pin), zSpeed(speed)
{
  zServo.attach(servoPin);
}

int ZAxis::movePosition(float position)
{
  int z_diameter = ZGEAR_DIAMETER;
  int lift = position;
  float angle = 0;
  angle = 180 - (lift*360)/(3.1415*z_diameter);
  if((angle >=0) || (angle <= 180))
  {
    zServo.write(angle, zSpeed);
  }
  else
    return 1;

  return 0;
}

void ZAxis::moveUp()
{
  zServo.write(0, zSpeed);
}

void ZAxis::moveDown()
{
  zServo.write(180, zSpeed);
}

void ZAxis::setSpeed(int speed)
{
  zSpeed = speed;
  return;
}
