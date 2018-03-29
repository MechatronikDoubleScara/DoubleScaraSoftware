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

void MajorAxis::init()
{
  float currentP1, currentP2;

  currentP1 = angleSensor1->getRotation();
  currentP2 = angleSensor2->getRotation();

  if((currentP1 < 180) && (currentP1 > 0))// && (currentP2 < 180) && (currentP2 > 0))
    moveLinksSingleStep((180.0 - currentP1 - INIT_ANGLE_LINK)/(360.0/NUMBER_SINGLE_STEPS), (currentP2 - INIT_ANGLE_LINK)/(360.0/NUMBER_SINGLE_STEPS));
  else if((currentP1 < 0) )//&& (currentP2 < 0))
    moveLinksSingleStep(( - (currentP1 + (180 - INIT_ANGLE_LINK)))/(360.0/NUMBER_SINGLE_STEPS), ( - (currentP2 + INIT_ANGLE_LINK))/(360.0/NUMBER_SINGLE_STEPS));

  //Serial.println(angleSensor1->getRotation());
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
