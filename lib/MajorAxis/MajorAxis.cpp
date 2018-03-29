#include <MajorAxis.h>




void MajorAxis::init()
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
}

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


  //Safety Distance from widest Z-Axis Point to avoid crashes with Motors -> 33mm + 2mm safety
  SafetyDistanceTCP = 35;
  //Calculate Distance TCP - Motors (Safety Area around Motors to avoid crashes of Z-Axis with Motors)
  distanceTCP1  = sqrt(pow(X - (-BASE_LENGTH/2 - NEMA/2), 2) + pow(Y - NEMA/2, 2)); // Top Left
  distanceTCP2  = sqrt(pow(X - (+BASE_LENGTH/2 + NEMA/2), 2) + pow(Y - NEMA/2, 2)); // Top Right
  distanceTCP3  = sqrt(pow(X - (-BASE_LENGTH/2 - NEMA/2), 2) + pow(Y - (-NEMA/2), 2)); // Bottom Left
  distanceTCP4  = sqrt(pow(X - (+BASE_LENGTH/2 + NEMA/2), 2) + pow(Y - (-NEMA/2), 2)); // Bottom Right


  if (D < DSafetyDistance)
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
