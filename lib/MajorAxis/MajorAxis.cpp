#include <MajorAxis.h>

Adafruit_StepperMotor *Stepper1;
Adafruit_StepperMotor *Stepper2;
Adafruit_MotorShield *AFMS;

void forwardstep1_SINGLE() { Stepper1->onestep(FORWARD, SINGLE); }
void backwardstep1_SINGLE() { Stepper1->onestep(BACKWARD, SINGLE); }
void forwardstep2_SINGLE() { Stepper2->onestep(FORWARD, SINGLE); }
void backwardstep2_SINGLE() { Stepper2->onestep(BACKWARD, SINGLE); }
void forwardstep1_MICRO() { Stepper1->onestep(FORWARD, MICROSTEP); }
void backwardstep1_MICRO() { Stepper1->onestep(BACKWARD, MICROSTEP); }
void forwardstep2_MICRO() { Stepper2->onestep(FORWARD, MICROSTEP); }
void backwardstep2_MICRO() { Stepper2->onestep(BACKWARD, MICROSTEP); }

MajorAxis::MajorAxis()
{
  LengthLeft = 0;
  LengthRight = 0;
  alpha1 = 0;
  alpha2 = 0;
  PHI1 = 0;
  PHI1d = 0;
  beta1 = 0;
  beta2 = 0;
  PHI4 = 0;
  PHI4d = 0;
  area = 0;

  //Sensor initialization
  angleSensor1 = new AS5048A(PIN_SS_SENSOR1);
  angleSensor2 = new AS5048A(PIN_SS_SENSOR2);

  angleSensor1->init();
  angleSensor2->init();

  angleSensor1->setZeroPosition(SENSOR1_OFFSET);
  angleSensor2->setZeroPosition(SENSOR2_OFFSET);

  angleSensor1->getRotation();
  angleSensor2->getRotation();

  AFMS = new Adafruit_MotorShield();

  Stepper1 = AFMS->getStepper(200, 2);
  Stepper2 = AFMS->getStepper(200, 1);

  AFMS->begin();

  stepper1_Single = new AccelStepper(forwardstep1_SINGLE, backwardstep1_SINGLE);
  stepper2_Single = new AccelStepper(forwardstep2_SINGLE, backwardstep2_SINGLE);
  stepper1_Micro = new AccelStepper(forwardstep1_MICRO, backwardstep1_MICRO);
  stepper2_Micro = new AccelStepper(forwardstep2_MICRO, backwardstep2_MICRO);

  stepper1_Single->setMaxSpeed(STEPPER_MAXSPEED);
  stepper1_Single->setAcceleration(STEPPER_ACCELERATION);
  stepper2_Single->setMaxSpeed(STEPPER_MAXSPEED);
  stepper2_Single->setAcceleration(STEPPER_ACCELERATION);
  stepper1_Micro->setMaxSpeed(STEPPER_MAXSPEED);
  stepper1_Micro->setAcceleration(STEPPER_ACCELERATION);
  stepper2_Micro->setMaxSpeed(STEPPER_MAXSPEED);
  stepper2_Micro->setAcceleration(STEPPER_ACCELERATION);
}

void MajorAxis::get()
{
  Serial.println(angleSensor1->getRotation());
}

void MajorAxis::init()
{
  //float currentP1, currentP2;

  //currentP1 = angleSensor1->getRotation();
  //currentP2 = angleSensor2->getRotation();

  /*
  if((currentP1 < 180) && (currentP1 > 0) &&(currentP2 < 180) && (currentP2 > 0))
    moveLinksSingleStep((180.0 - currentP1 - INIT_ANGLE_LINK)/(360.0/NUMBER_SINGLE_STEPS), (INIT_ANGLE_LINK - currentP2)/(360.0/NUMBER_SINGLE_STEPS));
  else if((currentP2 < 0) && (currentP1 < 0))
    moveLinksSingleStep(( - (currentP1 + (180 - INIT_ANGLE_LINK)))/(360.0/NUMBER_SINGLE_STEPS), ( - (INIT_ANGLE_LINK + currentP2))/(360.0/NUMBER_SINGLE_STEPS));
*/
  moveToAngle(150, 30);

  currentArea = 1;
  currentPosX = 999;
  currentPosY = 999;
}

int MajorAxis::movePosition(float X, float Y)
{
  if(calculateAngles(X, Y) < 0)
    return -1;

  if(currentArea != area)
  {
    if(currentArea == 1)
    {
      moveToAngle(150, 30);
      delay(100);
      moveToAngle(-150, -30);
    }
    else if(currentArea == 2)
    {
      moveToAngle(-150, -30);
      delay(100);
      moveToAngle(150, 30);
    }
  }

  moveToAngle(PHI1d, PHI4d);

  currentArea = area;
  currentPosX = X;
  currentPosY = Y;

  return 0;
}

void MajorAxis::moveToAngle(double alpha, double beta)
{
  double a1, a2;

  a1 = calculateToGoAngle(alpha, 1);
  a2 = calculateToGoAngle(beta, 2);
  moveLinksSingleStep(a1/(360.0/NUMBER_SINGLE_STEPS), a2/(360.0/NUMBER_SINGLE_STEPS));

  //a1 = calculateToGoAngle(alpha, 1);
  //a2 = calculateToGoAngle(beta, 2);
  //moveLinksMicroStep(a1/(360.0/NUMBER_MICRO_STEPS), a2/(360.0/NUMBER_MICRO_STEPS));
}

double MajorAxis::calculateToGoAngle(double targetAngle, int motoridx)
{
  double toGoAngle;
  double currentAngle;
  if(motoridx == 1)
  {
    currentAngle = angleSensor1->getRotation();

    if(((targetAngle > 0) && (currentAngle > 0)) || ((targetAngle < 0) && (currentAngle < 0)))
      toGoAngle = targetAngle - currentAngle;
    else if((targetAngle < 0) && (currentAngle > 0))
      toGoAngle = 360.0 - currentAngle + targetAngle;
    else if((targetAngle > 0) && (currentAngle < 0))
      toGoAngle = - currentAngle + targetAngle - 360.0;
  }
  else if(motoridx == 2)
  {
    currentAngle = angleSensor2->getRotation();

    toGoAngle = targetAngle - currentAngle;
  }

  return toGoAngle;
}

void MajorAxis::moveLinksSingleStep(int steps1, int steps2)
{
  stepper1_Single->move(steps1);
  stepper2_Single->move(steps2);
  while((stepper1_Single->distanceToGo()!=0) || (stepper2_Single->distanceToGo()!=0))
  {
    stepper1_Single->run();
    stepper2_Single->run();
  }
}

void MajorAxis::moveLinksMicroStep(int steps1, int steps2)
{
  stepper1_Micro->move(steps1);
  stepper2_Micro->move(steps2);
  while((stepper1_Micro->distanceToGo()!=0) || (stepper2_Micro->distanceToGo()!=0))
  {
    stepper1_Micro->run();
    stepper2_Micro->run();
  }
}

void MajorAxis::setZeroPositionLinks(float offset1, float offset2)
{
  sensor_offset1 = offset1;
  sensor_offset2 = offset2;

  angleSensor1->setZeroPosition(offset1);
  angleSensor2->setZeroPosition(offset2);
}

/*void MajorAxis::setMovmentParameter(float speed, float acc)
{

}*/

double MajorAxis::getAngle1()
{
  return PHI1d;
}

double MajorAxis::getAngle4()
{
  return PHI4d;
}

int MajorAxis::calculateAngles(double X, double Y)
{
  Serial.print("Start angle calculation with X = ");
  Serial.print(X);
  Serial.print(" and Y = ");
  Serial.println(Y);

  if (Y >= 0.00)
  {
    area = 1; // robot moving in top area
  }
  else if (Y < 0.00)
  {
    area = 2; // robot moving in bottom area
  }
  else
  {
    Serial.println("Error");
    return -1; //error
  }


  if (area == 1)
  {
    Serial.println("Calculate angles for top area");

    LengthLeft  = sqrt(pow(BASE_LENGTH/2 + X, 2) + pow(Y,2));
    LengthRight = sqrt(pow(BASE_LENGTH/2 - X, 2) + pow(Y,2));

    alpha2      = atan2(Y, BASE_LENGTH/2 + X);
    alpha1      = acos((pow(LINK1_LENGTH, 2) + pow(LengthLeft, 2) - pow(LINK2_LENGTH, 2)) / (2*LINK1_LENGTH*LengthLeft));
    PHI1        = alpha1 + alpha2;

    // norm to 0 ... 360
    if(PHI1 >= 2*PI)
    {
      PHI1 = PHI1 - 2*PI;
    }
    else if (PHI1 < 0)
    {
      PHI1 = PHI1 + 2*PI;
    }

    // scale to 0 ... 180, -180 ... 0
    if (PHI1 > PI)
    {
      PHI1 = PHI1 - 2*PI;
    }
    PHI1d       = PHI1 * 360/(2*PI);

    beta2       = atan2(Y, BASE_LENGTH/2 - X);
    beta1       = acos((pow(LINK1_LENGTH, 2) + pow(LengthRight, 2) - pow(LINK2_LENGTH, 2)) / (2*LINK1_LENGTH*LengthRight));
    PHI4        = PI - beta2 - beta1;

    // norm to 0 ... 360
    if(PHI4 >= 2*PI)
    {
      PHI4 = PHI4 - 2*PI;
    }
    else if (PHI4 < 0)
    {
      PHI4 = PHI4 + 2*PI;
    }

    // scale to 0 ... 180, -180 ... 0
    if (PHI4 > PI)
    {
      PHI4 = PHI4 - 2*PI;
    }
    PHI4d       = PHI4 * 360/(2*PI);
  }
  else if(area == 2)
  {
    Serial.println("Calculate angles for bottom area");

    LengthLeft  = sqrt(pow(BASE_LENGTH/2 + X, 2) + pow(-Y, 2));
    LengthRight = sqrt(pow(BASE_LENGTH/2 - X, 2) + pow(-Y, 2));

    alpha2      = atan2(-Y, BASE_LENGTH/2 + X);
    alpha1      = acos((pow(LINK1_LENGTH, 2) + pow(LengthLeft, 2) - pow(LINK2_LENGTH, 2)) / (2*LINK1_LENGTH*LengthLeft));
    PHI1        = 2*PI - alpha1 - alpha2;

    // norm to 0 ... 360
    if(PHI1 >= 2*PI)
    {
      PHI1 = PHI1 - 2*PI;
    }
    else if (PHI1 < 0)
    {
      PHI1 = PHI1 + 2*PI;
    }

    // scale to 0 ... 180, -180 ... 0
    if (PHI1 > PI)
    {
      PHI1 = PHI1 - 2*PI;
    }
    PHI1d       = PHI1 * 360/(2*PI);

    beta2       = atan2(-Y, BASE_LENGTH/2 - X);
    beta1       = acos((pow(LINK1_LENGTH, 2) + pow(LengthRight, 2) - pow(LINK2_LENGTH, 2)) / (2*LINK1_LENGTH*LengthRight));
    //PHI4        = -(PI - beta2 - beta1);
    PHI4        = PI + beta2 + beta1;

    // norm to 0 ... 360
    if(PHI4 >= 2*PI)
    {
      PHI4 = PHI4 - 2*PI;
    }
    else if (PHI4 < 0)
    {
      PHI4 = PHI4 + 2*PI;
    }

    // scale to 0 ... 180, -180 ... 0
    if (PHI4 > PI)
    {
      PHI4 = PHI4 - 2*PI;
    }
    PHI4d       = PHI4 * 360/(2*PI);
  }
  else
  {
    return -1;
  }

  if (isnan(PHI1) || isnan(PHI4))
  {
    Serial.println("Coordinate not in workspace");
    return -1; // one angle is nan -> position out of workspace
  }

  // Distance D with safety distance (crash between joints)
  DSafetyDistance = 20;
  // Calculation of D (disance between joints)
  Ax = -BASE_LENGTH/2 + LINK1_LENGTH*cos(PHI1);
  Ay = 0 + LINK1_LENGTH*sin(PHI1);
  Bx = BASE_LENGTH/2 + LINK1_LENGTH*cos(PHI4);
  By = 0 + LINK1_LENGTH*sin(PHI4);
  D = sqrt(pow(Ax-Bx, 2) + pow(Ay-By, 2));


  //Calculate distance from 1. axis to motor axis
  motorSafetyDistance = 30;
  ArmDistanceLeft =   sqrt(pow(Ax - BASE_LENGTH/2, 2) + pow(Ay-0, 2));
  ArmDistanceRight =  sqrt(pow(Bx + BASE_LENGTH/2, 2) + pow(By-0, 2));


  //Safety Distance from widest Z-Axis Point to avoid crashes with Motors -> 33mm + 2mm safety
  SafetyDistanceTCP = 35;
  //Calculate Distance TCP - Motors (Safety Area around Motors to avoid crashes of Z-Axis with Motors)
  distanceTCP1  = sqrt(pow(X - (-BASE_LENGTH/2 - NEMA/2), 2) + pow(Y - NEMA/2, 2)); // Top Left
  distanceTCP2  = sqrt(pow(X - (+BASE_LENGTH/2 + NEMA/2), 2) + pow(Y - NEMA/2, 2)); // Top Right
  distanceTCP3  = sqrt(pow(X - (-BASE_LENGTH/2 - NEMA/2), 2) + pow(Y - (-NEMA/2), 2)); // Bottom Left
  distanceTCP4  = sqrt(pow(X - (+BASE_LENGTH/2 + NEMA/2), 2) + pow(Y - (-NEMA/2), 2)); // Bottom Right


  if (ArmDistanceLeft < motorSafetyDistance || ArmDistanceRight < motorSafetyDistance)
  {
    Serial.println("Arm-Position too near at Motor - STOP");
    return -2;
  }
  else if (D < DSafetyDistance)
  {
    Serial.println("Left Joint too near to right Joint - STOP");
    return -2;
  }
  else if (X >= (-BASE_LENGTH/2 - NEMA/2) && X <= (BASE_LENGTH/2 + NEMA/2) && Y <= (NEMA/2 + SafetyDistanceTCP) && Y >= (-NEMA/2 - SafetyDistanceTCP))
  {
    Serial.println("TCP in saftey area - only accessable with predefined movement - STOP");
    return -2;
  }
  else if (X >= (-BASE_LENGTH/2 - NEMA/2 - SafetyDistanceTCP) && X <= (BASE_LENGTH/2 + NEMA/2 + SafetyDistanceTCP) && Y <= (NEMA/2) && Y >= (-NEMA/2))
  {
    Serial.println("TCP in saftey area - only accessable with predefined movement - STOP");
    return -2;
  }
  else if (distanceTCP1 <= SafetyDistanceTCP || distanceTCP2 <= SafetyDistanceTCP || distanceTCP3 <= SafetyDistanceTCP || distanceTCP4 <= SafetyDistanceTCP)
  {
    Serial.println("TCP in saftey area - only accessable with predefined movement - STOP");
    return -2;
  }

  return 1;
}

void MajorAxis::printAngles()
{
  Serial.print("PHI1: ");
  Serial.print(PHI1d);
  Serial.println("°");
  Serial.print("PHI4: ");
  Serial.print(PHI4d);
  Serial.println("°");
}
