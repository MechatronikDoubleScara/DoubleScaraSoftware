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

    LengthLeft = sqrt((BASE_LENGTH/2 + X)*(BASE_LENGTH/2 + X) + Y*Y);
    LengthRight = sqrt((BASE_LENGTH/2 - X)*(BASE_LENGTH/2 - X) + Y*Y);

    alpha2 = atan2(Y, BASE_LENGTH/2 + X);
    alpha1 = acos((LINK1_LENGTH*LINK1_LENGTH + LengthLeft*LengthLeft - LINK2_LENGTH*LINK2_LENGTH) / (2*LINK1_LENGTH*LengthLeft));
    PHI1 = alpha1 + alpha2;
    PHI1d = PHI1 * 360/(2*PI);

    beta2 = atan2(Y, BASE_LENGTH/2 - X);
    beta1 = acos((LINK1_LENGTH*LINK1_LENGTH + LengthRight*LengthRight - LINK2_LENGTH*LINK2_LENGTH) / (2*LINK1_LENGTH*LengthRight));
    PHI4 = PI - beta2 - beta1;
    PHI4d = PHI4 * 360/(2*PI);
  }
  else if(area == 2)
  {
    Serial.println("Calculate angles for bottom area");

    LengthLeft = sqrt((BASE_LENGTH/2 + X)*(BASE_LENGTH/2 + X) + (-Y*-Y));
    LengthRight = sqrt((BASE_LENGTH/2 - X)*(BASE_LENGTH/2 - X) + (-Y*-Y));

    alpha2 = atan2(-Y, BASE_LENGTH/2 + X);
    alpha1 = acos((LINK1_LENGTH*LINK1_LENGTH + LengthLeft*LengthLeft - LINK2_LENGTH*LINK2_LENGTH) / (2*LINK1_LENGTH*LengthLeft));
    PHI1 = 2*PI - alpha1 - alpha2;
    PHI1d = PHI1 * 360/(2*PI);

    beta2 = atan2(-Y, BASE_LENGTH/2 - X);
    beta1 = acos((LINK1_LENGTH*LINK1_LENGTH + LengthRight*LengthRight - LINK2_LENGTH*LINK2_LENGTH) / (2*LINK1_LENGTH*LengthRight));
    PHI4 = PI + beta2 + beta1;
    PHI4d = PHI4 * 360/(2*PI);
  }
  else
  {
    return -1;
  }


  if (isnan(PHI1) || isnan(PHI4))
  {
    return -1;
  }
  else
  {
    return 1;
  }

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
