#include <Communication.h>

void Communication::init()
{
  command = ' ';
  recievedData = ""; // contains the whole string of recieved data from bluetooth connection
  temp = ""; // contains the temporary sub messages that were split up
  mode = -1;

  // Initialise variables MODE 1
  X = 0.0;
  Y = 0.0;
  Z = 0.0;
  magnet = 0;
  // Initialise variables MODE 1

  // Initialise variables MODE 1


  // Initialise serial communications
  Serial.begin(9600);
  Serial.println("New Session started!");
  bluetooth.begin(9600);
}

void Communication::print(String message)
{
    bluetooth.print(message);
}


void Communication::recieve()
{
  recievedData = "";

  while(bluetooth.available() > 0)
  {
    command = bluetooth.read();

    if(command == '~')
    {
      recievedData += command;
      break;
    }
    else
    {
      recievedData += command;
    }

    delay(1);

    //PRINT
    /*
    Serial.print("Recieved Data: ");
    Serial.println(recievedData);
    Serial.print("Lenght: ");
    Serial.println(recievedData.length());
    */
  }

  if(recievedData.length() > 2 && recievedData[0] == '#' && recievedData[recievedData.length()-1] == '~')
  {
    Serial.println(recievedData);

    // get mode from recievedData
    int i = 1; // start with character 1 (character 0 is check value)
    int j = 0; // start with first string array entry


    while(recievedData[i] != '~')
    {
      temp = ""; // reset temp string
      while(recievedData[i] != '_')
      {
        temp += recievedData[i];
        i++;
      }
      //Serial.print("temp:");
      //Serial.println(temp);
      data[j] = temp;
      //Serial.print("data:");
      //Serial.println(data[j]);
      i++;
      j++;
    }

    mode = data[0].toInt();

    if(mode == 0)
    {
        // EXTRACT FROM MESSAGE FOR MODE 0
        activity = data[1].toInt();

        Serial.print("Activity: ");
        Serial.println(activity);
    }
    else if(mode == 1)
    {
      // EXTRACT FROM MESSAGE FOR MODE 1
      X       = data[1].toDouble();
      Y       = data[2].toDouble();
      Z       = data[3].toDouble();
      magnet  = data[4].toInt();

      Serial.print("Mode: ");
      Serial.println(mode);
      Serial.print("X: ");
      Serial.println(X);
      Serial.print("Y: ");
      Serial.println(Y);
      Serial.print("Z: ");
      Serial.println(Z);
      Serial.print("Magnet: ");
      Serial.println(magnet);
    }
    else if(mode == 2)
    {
      // EXTRACT FROM MESSAGE FOR MODE 2
      shape   = data[1].toInt();

      Serial.print("Shape: ");
      Serial.println(shape);
    }
    else if(mode == 3)
    {
      // EXTRACT FROM MESSAGE FOR MODE 3
      player  = data[1].toInt();
      row     = data[2].toInt();
      col     = data[3].toInt();

      Serial.print("Player: ");
      Serial.println(player);
      Serial.print("Row: ");
      Serial.println(row);
      Serial.print("Col: ");
      Serial.println(col);
    }
    else
    {
      Serial.println("Mode UNKNOWN!");
    }
  }
}


void Communication::resetRecievedData()
{
  recievedData = "";
  command = ' ';
  mode = -1;
}

int Communication::getMode()
{
  return mode;
}

double Communication::getCoordinate(char axis)
{
  switch(axis)
  {
    case 'X':
      return X;
    case 'Y':
      return Y;
    case 'Z':
      return Z;
    default:
      return 0.0;
  }

}

int Communication::getActivity()
{
  return activity;
}
