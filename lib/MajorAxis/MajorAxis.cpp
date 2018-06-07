#include <MajorAxis.h>

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

  stepper1 = new AccelStepper(1, PIN_MOTOR1_STEPS, PIN_MOTOR1_DIR);
  stepper2 = new AccelStepper(1, PIN_MOTOR2_STEPS, PIN_MOTOR2_DIR);

  stepper1->setMaxSpeed(STEPPER_MAXSPEED);
  stepper1->setSpeed(STEPPER_SPEED);
  stepper1->setAcceleration(STEPPER_ACCELERATION);
  stepper2->setMaxSpeed(STEPPER_SPEED);
  stepper2->setSpeed(STEPPER_MAXSPEED);
  stepper2->setAcceleration(STEPPER_ACCELERATION);

  init();
}

void MajorAxis::init()
{
  double a1 = getRotationMean(1);
  //delay(400);
  double a2 = getRotationMean(2);
  delay(400);

  if(a1 > 0 && a2 > 0)
  {
    currentArea = 1;
    moveToAngle(150, 30);
  }
  else if(a1 < 0 && a2 < 0)
  {
    currentArea = -1;
    moveToAngle(-150, -30);
  }
  else
    Serial.println("Error");
}

// Move TCP to absolut X & Y Position
int MajorAxis::movePosition(float X, float Y, bool correction = true)
{
  calculateAngles(X, Y);

  // check if saftey positions are neccessary to reach destined coordinates

  if(currentArea != area)
  {
    if(currentArea == 2)
    {
      // SAFE Position 2
      moveToAngle(-144.3, 127.2); // angles for Position (X: -115, Y: 70)
    }
    else if (currentArea == 3)
    {
      // SAFE Position 3
      moveToAngle(52.8, -35.7); // angles for Position (X: 115, Y: 70)
    }
    else if (currentArea == -2)
    {
      // SAFE Position -2
      moveToAngle(144.3, -127.2); // angles for Position (X: -115, Y: -70)
    }
    else if (currentArea == -3)
    {
      // SAFE Position -3
      moveToAngle(-52.8, 35.7); // angles for Position (X: 115, Y: -70)
    }

    delay(400);

    if((currentArea < 0 && area > 0) || (currentArea > 0 && area < 0))
    {
      changeSide();
    }

    delay(400);

    if(area == 2)
    {
      // SAFE Position 2
      moveToAngle(-144.3, 127.2); // angles for Position (X: -115, Y: 60)
    }
    else if (area == 3)
    {
      // SAFE Position 3
      moveToAngle(52.8, -35.7); // angles for Position (X: 115, Y: 70)
    }
    else if (area == -2)
    {
      // SAFE Position -2
      moveToAngle(144.3, -127.2); // angles for Position (X: -115, Y: -70)
    }
    else if (area == -3)
    {
      // SAFE Position -3
      moveToAngle(-52.8, 35.7); // angles for Position (X: 115, Y: -70)
    }
  }

  delay(400);

  // angles for position from user input
  moveToAngle(PHI1d, PHI4d, correction);

  currentArea = area;
  currentPosX = X;
  currentPosY = Y;

  return 0;
}

// Move both links to an absolut angle
void MajorAxis::moveToAngle(double alpha, double beta, bool correction)
{
  double a1, a2, c1, c2;
  int steps_m1, steps_m2, csteps_m1, csteps_m2;

  //First Movement
  a1 = calculateToGoAngle(alpha, 1);
  a2 = calculateToGoAngle(beta, 2);

  steps_m1 = a1/(360.0/NUMBER_EIGHTH_STEPS);
  steps_m2 = a2/(360.0/NUMBER_EIGHTH_STEPS);

  moveLinksStep(steps_m1, steps_m2);

  Serial.print("First-Steps M1: ");
  Serial.println(steps_m1);
  Serial.print("First-Steps M2: ");
  Serial.println(steps_m2);

  //Correction Movement
  if(correction == true)
    {
      delay(400);
      setMovementParameter(50, 50, 50);

      c1 = calculateToGoAngle(alpha, 1);
      c2 = calculateToGoAngle(beta, 2);

      csteps_m1 = c1/(360.0/NUMBER_EIGHTH_STEPS);
      csteps_m2 = c2/(360.0/NUMBER_EIGHTH_STEPS);

      moveLinksStep(csteps_m1, csteps_m2);

      Serial.print("Correction-Steps M1: ");
      Serial.println(csteps_m1);
      Serial.print("Correction-Steps M2: ");
      Serial.println(csteps_m2);

      setMovementParameter(STEPPER_MAXSPEED, STEPPER_SPEED, STEPPER_ACCELERATION);
  }
}

// Calculate toGoAngle from current and goal angle
double MajorAxis::calculateToGoAngle(double targetAngle, int motoridx)
{
  double toGoAngle;
  double currentAngle;
  if(motoridx == 1)
  {
    currentAngle = getRotationMean(motoridx);

    if(((targetAngle > 0) && (currentAngle > 0)) || ((targetAngle < 0) && (currentAngle < 0)))
      toGoAngle = targetAngle - currentAngle;
    else if((targetAngle < 0) && (currentAngle > 0))
      toGoAngle = 360.0 - currentAngle + targetAngle;
    else if((targetAngle > 0) && (currentAngle < 0))
      toGoAngle = - currentAngle + targetAngle - 360.0;

      Serial.print("toGoAngle M1: ");
      Serial.println(toGoAngle);
  }
  else if(motoridx == 2)
  {
    currentAngle = getRotationMean(motoridx);

    toGoAngle = targetAngle - currentAngle;

    Serial.print("toGoAngle M2: ");
    Serial.println(toGoAngle);
  }

  return toGoAngle;
}

// Loop over some sensor measurments and get mean value
double MajorAxis::getRotationMean(int motoridx)
{
  double currentAngle = 0;
  double meanRotation;
  int loopsize = 20;

  if(motoridx == 1)
  {
    for(int i = 0; i < loopsize; i++){
      currentAngle = currentAngle + angleSensor1->getRotation();
      delay(5);
    }
  }
  else if(motoridx == 2)
  {
    for(int i = 0; i < loopsize; i++){
      currentAngle = currentAngle + angleSensor2->getRotation();
      delay(5);
    }
  }

  meanRotation = currentAngle / loopsize;
  return meanRotation;
}

// Move both links a specific amount of steps
void MajorAxis::moveLinksStep(int steps1, int steps2)
{
  stepper1->move(steps1);
  stepper2->move(steps2);
  while((stepper1->distanceToGo()!=0) || (stepper2->distanceToGo()!=0))
  {
    stepper1->run();
    stepper2->run();
  }
}

// Routine for changing side. Positive <-> Negative side
void MajorAxis::changeSide()
{
  if(currentArea > 0)
  {
    delay(400);
    moveToAngle(130, 50);
    delay(400);
    setMovementParameter(8000, 8000, 12000);
    moveToAngle(-130, -50, false);
    delay(400);
  }
  else if(currentArea < 0)
  {
    delay(400);
    moveToAngle(-130, -50);
    delay(400);
    setMovementParameter(8000, 8000, 12000);
    moveToAngle(130, 50, false);
    delay(400);
  }
  setMovementParameter(STEPPER_MAXSPEED, STEPPER_SPEED, STEPPER_ACCELERATION);
}

// Set offset value for both rotation sensors
void MajorAxis::setSensorOffset(float offset1, float offset2)
{
  sensor_offset1 = offset1;
  sensor_offset2 = offset2;

  angleSensor1->setZeroPosition(offset1);
  angleSensor2->setZeroPosition(offset2);
}

// Set movment parameters for both motors. Speed, Maxspeed & Acceleration
void MajorAxis::setMovementParameter(int speed, int maxspeed, int acc)
{
  stepper1->setMaxSpeed(maxspeed);
  stepper1->setSpeed(speed);
  stepper1->setAcceleration(acc);
  stepper2->setMaxSpeed(maxspeed);
  stepper2->setSpeed(speed);
  stepper2->setAcceleration(acc);
}

// Print raw sensorvalues
void MajorAxis::printSensorValueRaw()
{
  Serial.print("Sensor1:");
  Serial.println(angleSensor1->getRotation());
  Serial.print("Sensor2:");
  Serial.println(angleSensor2->getRotation());
  delay(500);
}

// Print mean sensorvalues
void MajorAxis::printSensorValueMean()
{
  Serial.print("Sensor1:");
  Serial.println(getRotationMean(1));
  Serial.print("Sensor2:");
  Serial.println(getRotationMean(2));
  delay(500);
}

double MajorAxis::getAngle1(){
  return PHI1d;
}

double MajorAxis::getAngle4(){
  return PHI4d;
}

int MajorAxis::calculateAngles(double X, double Y)
{
  Serial.print("Start angle calculation with X = ");
  Serial.print(X);
  Serial.print(" and Y = ");
  Serial.println(Y);

  if (Y >= 0.00){

    if(X < 0 && Y < 56){
      area = 2; // robot moving in top left area
    }
    else if(X > 0 && Y < 56){
      area = 3; // robot moving in top right area
    }
    else{
      area = 1; // robot moving in top area
    }

  }
  else{
    if(X < 0 && Y > -56){
      area = -2; // robot moving in bottom left area
    }
    else if(X > 0 && Y > -56){
      area = -3; // robot moving in bottom right area
    }
    else{
      area = -1; // robot moving in bottom area
    }
  }

  if (area > 0)
  {
    Serial.println("Calculate angles for top area");

    LengthLeft  = sqrt(pow(BASE_LENGTH/2 + X, 2) + pow(Y,2));
    LengthRight = sqrt(pow(BASE_LENGTH/2 - X, 2) + pow(Y,2));

    alpha2      = atan2(Y, BASE_LENGTH/2 + X);
    alpha1      = acos((pow(LINK1_LENGTH, 2) + pow(LengthLeft, 2) - pow(LINK2_LENGTH, 2)) / (2*LINK1_LENGTH*LengthLeft));
    PHI1        = alpha1 + alpha2;

    // norm to 0 ... 360
    if(PHI1 >= 2*PI){
      PHI1 = PHI1 - 2*PI;
    }
    else if (PHI1 < 0){
      PHI1 = PHI1 + 2*PI;
    }

    // scale to 0 ... 180, -180 ... 0
    if (PHI1 > PI){
      PHI1 = PHI1 - 2*PI;
    }

    PHI1d       = PHI1 * 360/(2*PI);

    beta2       = atan2(Y, BASE_LENGTH/2 - X);
    beta1       = acos((pow(LINK1_LENGTH, 2) + pow(LengthRight, 2) - pow(LINK2_LENGTH, 2)) / (2*LINK1_LENGTH*LengthRight));
    PHI4        = PI - beta2 - beta1;

    // norm to 0 ... 360
    if(PHI4 >= 2*PI){
      PHI4 = PHI4 - 2*PI;
    }
    else if (PHI4 < 0){
      PHI4 = PHI4 + 2*PI;
    }

    // scale to 0 ... 180, -180 ... 0
    if (PHI4 > PI){
      PHI4 = PHI4 - 2*PI;
    }

    PHI4d       = PHI4 * 360/(2*PI);
  }
  else if(area < 0)
  {
    Serial.println("Calculate angles for bottom area");

    LengthLeft  = sqrt(pow(BASE_LENGTH/2 + X, 2) + pow(-Y, 2));
    LengthRight = sqrt(pow(BASE_LENGTH/2 - X, 2) + pow(-Y, 2));

    alpha2      = atan2(-Y, BASE_LENGTH/2 + X);
    alpha1      = acos((pow(LINK1_LENGTH, 2) + pow(LengthLeft, 2) - pow(LINK2_LENGTH, 2)) / (2*LINK1_LENGTH*LengthLeft));
    PHI1        = 2*PI - alpha1 - alpha2;

    // norm to 0 ... 360
    if(PHI1 >= 2*PI){
      PHI1 = PHI1 - 2*PI;
    }
    else if (PHI1 < 0){
      PHI1 = PHI1 + 2*PI;
    }

    // scale to 0 ... 180, -180 ... 0
    if (PHI1 > PI){
      PHI1 = PHI1 - 2*PI;
    }

    PHI1d       = PHI1 * 360/(2*PI);

    beta2       = atan2(-Y, BASE_LENGTH/2 - X);
    beta1       = acos((pow(LINK1_LENGTH, 2) + pow(LengthRight, 2) - pow(LINK2_LENGTH, 2)) / (2*LINK1_LENGTH*LengthRight));
    //PHI4        = -(PI - beta2 - beta1);
    PHI4        = PI + beta2 + beta1;

    // norm to 0 ... 360
    if(PHI4 >= 2*PI){
      PHI4 = PHI4 - 2*PI;
    }
    else if (PHI4 < 0){
      PHI4 = PHI4 + 2*PI;
    }

    // scale to 0 ... 180, -180 ... 0
    if (PHI4 > PI){
      PHI4 = PHI4 - 2*PI;
    }
    PHI4d       = PHI4 * 360/(2*PI);
  }
  else{
    return -1;
  }

  if (isnan(PHI1) || isnan(PHI4))
  {
    Serial.println("Coordinate not in workspace");
    return -2; // one angle is nan -> position out of workspace
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
    return -3;
  }
  else if (D < DSafetyDistance)
  {
    Serial.println("Left Joint too near to right Joint - STOP");
    return -3;
  }
  else if (X >= (-BASE_LENGTH/2 - NEMA/2) && X <= (BASE_LENGTH/2 + NEMA/2) && Y <= (NEMA/2 + SafetyDistanceTCP) && Y >= (-NEMA/2 - SafetyDistanceTCP))
  {
    Serial.println("TCP in saftey area - only accessable with predefined movement - STOP");
    return -3;
  }
  else if (X >= (-BASE_LENGTH/2 - NEMA/2 - SafetyDistanceTCP) && X <= (BASE_LENGTH/2 + NEMA/2 + SafetyDistanceTCP) && Y <= (NEMA/2) && Y >= (-NEMA/2))
  {
    Serial.println("TCP in saftey area - only accessable with predefined movement - STOP");
    return -3;
  }
  else if (distanceTCP1 <= SafetyDistanceTCP || distanceTCP2 <= SafetyDistanceTCP || distanceTCP3 <= SafetyDistanceTCP || distanceTCP4 <= SafetyDistanceTCP)
  {
    Serial.println("TCP in saftey area - only accessable with predefined movement - STOP");
    return -3;
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
