#include <ZAxis.h>

ZAxis::ZAxis(const int Pin = PIN_ZAXIS_SERVO, int speed = Z_SERVO_SPEED): servoPin(Pin), zSpeed(speed)
{
  zServo.attach(servoPin);
  posUp = Z_SERVO_POS_UP;
  posDown = Z_SERVO_POS_DOWN;
  moveUp();
}

int ZAxis::movePosition(float position)
{
  int z_diameter = ZGEAR_DIAMETER;
  int lift = position;
  float angle = 0;
  angle = 180 - (lift*360*Z_SERVO_CORRECTION_FACTOR)/(3.1415*z_diameter);
  if((angle >=0) && (angle <= 180))
  {
    zServo.write(angle, zSpeed);
  }
  else
    return 1;

  return 0;
}

int ZAxis::moveUp()
{
  return movePosition(posUp);
}

int ZAxis::moveDown()
{
  return movePosition(posDown);
}

void ZAxis::setSpeed(int speed)
{
  zSpeed = speed;
  return;
}

void ZAxis::setPosUp(float pos){
  posUp = pos;
  //TODO: check if falid
}

void ZAxis::setPosDown(float pos){
  posDown = pos;
  //TODO: check if falid
}
