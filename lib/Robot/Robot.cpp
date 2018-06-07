#include <Robot.h>

int Robot::movePosition(ObjectCarrier* ObjectCarrier, int x_pos, int y_pos, bool correction = true){
  float x_coord = ObjectCarrier->getCoordX(x_pos, y_pos);
  float y_coord = ObjectCarrier->getCoordY(x_pos, y_pos);

  if (abs(x_coord) < 1 || abs(y_coord) < 1 ){
    return -1;
  }
  MajorAxis->movePosition(x_coord, y_coord, correction);
  return 0;
}


int Robot::movePart(ObjectCarrier* ObjectCarrier1, int x1_pos, int y1_pos, ObjectCarrier* ObjectCarrier2, int x2_pos, int y2_pos, int lockType=0){

  if(ObjectCarrier1->getState(x1_pos, y1_pos) == -1){
    Serial.println("Error: can not move part (Pos1 not allowed)");
    return -1;
  }
  else if(ObjectCarrier2->getState(x2_pos, y2_pos) == -1){
    Serial.println("Error: can not move part (Pos2 not allowed)");
    return -1;
  }
  else if (ObjectCarrier1->getState(x1_pos, y1_pos) <= 0){
    Serial.println("Error: can not move part (no part at Pos1)");
    return -2;
  }
  else if (ObjectCarrier2->getState(x2_pos, y2_pos) > 0){
    Serial.println("Error: can not move part (Pos2 allready occupied)");
    return -3;
  }


  float x1_coord = ObjectCarrier1->getCoordX(x1_pos, y1_pos);
  float y1_coord = ObjectCarrier1->getCoordY(x1_pos, y1_pos);
  int type = ObjectCarrier1->getState(x1_pos, y1_pos);

  float x2_coord = ObjectCarrier2->getCoordX(x2_pos, y2_pos);
  float y2_coord = ObjectCarrier2->getCoordY(x2_pos, y2_pos);


  if (abs(x1_coord) < 1 || abs(y1_coord) < 1 || abs(x2_coord) < 1 || abs(y2_coord) < 1)
    return -1;

  MajorAxis->movePosition(x1_coord, y1_coord);
  ZAxis->moveDown();
  delay(1000);
  Endeffector->setState(true);
  delay(250);
  ZAxis->moveUp();
  delay(500);
  ObjectCarrier1->resetPosition(x1_pos, y1_pos, lockType);


  MajorAxis->movePosition(x2_coord, y2_coord);
  ZAxis->moveDown();
  delay(500);
  Endeffector->setState(false);
  delay(250);
  ZAxis->moveUp();
  delay(500);
  ObjectCarrier2->setPosition(x2_pos, y2_pos, type);

  return 0;
}


int Robot::cleanPlate(ObjectCarrier* ObjectCarrier1, ObjectCarrier* ObjectCarrier2){ //clean ObjectCarrier1 and store elements in ObjectCarrier2
  for (int type = 1; (!ObjectCarrier1->isEmpty()) && (type <= OBJECTCARRIER_NUM_MAX_TYPES); type++){
      while(1){
        int x1_pos, y1_pos, x2_pos, y2_pos;
        if(ObjectCarrier1->nextOccupiedPosition(x1_pos, y1_pos, type) != 0){
          break;
        }
        if(ObjectCarrier2->nextFreePosition(x2_pos, y2_pos, type) != 0){
          Serial.print("Error: unable to clean plate (no free place left)\n");
          return -1;
        }
        movePart(ObjectCarrier1, x1_pos, y1_pos, ObjectCarrier2, x2_pos, y2_pos, 0);
      }
  }
  if (ObjectCarrier1->isEmpty()){
    return 0;
  }
  else{
    Serial.print("Error: unable to clean plate (unknown error)\n");
    return-1;
  }
}

int Robot::fillPlate(ObjectCarrier* ObjectCarrier1, ObjectCarrier* ObjectCarrier2){
  for (int type = 1; (!ObjectCarrier2->isFull()) && (type <= OBJECTCARRIER_NUM_MAX_TYPES); type++){
      while(1){
        int x1_pos, y1_pos, x2_pos, y2_pos;
        if(ObjectCarrier2->nextFreePosition(x2_pos, y2_pos, type) != 0){
          break;
        }
        if(ObjectCarrier1->nextOccupiedPosition(x1_pos, y1_pos, type) != 0){
          Serial.print("Error: unable to fill plate (no suitable objects left)\n");
          return -1;
        }
        movePart(ObjectCarrier1, x1_pos, y1_pos, ObjectCarrier2, x2_pos, y2_pos, 1);
      }
  }
  if (ObjectCarrier2->isFull()){
    return 0;
  }
  else{
    Serial.print("Error: unable to fill plate (unknown error)\n");
    return-1;
  }
}

int Robot::makeShape(ObjectCarrier* ObjectCarrier1, ObjectCarrier* ObjectCarrier2, int shape){
  switch(shape){
    case SHAPE_ARROW:
      ObjectCarrier2->setMode(OBJECTCARRIER_SHAPE_ARROW);
      Serial.println("Object carrier SHAPE_ARROW loded");
      break;
    case SHAPE_SMILEY:
      ObjectCarrier2->setMode(OBJECTCARRIER_SHAPE_SMILEY);
      Serial.println("Object carrier SHAPE_SMILEY loded");
      break;
    default:
      Serial.println("Error: Can not buid shape (Unknown Shape)");
      return -1;
      break;
  }
  ObjectCarrier1->setMode(OBJECTCARRIER_MAGAZINE);
  Serial.println("Object carrier MAGAZINE loded");

  int success = fillPlate(ObjectCarrier1, ObjectCarrier2);
  if(success == 0){
    Serial.println("Shape successfully buid");
  }
  return success;
}

int Robot::ticTacToeInit(ObjectCarrier* ObjectCarrier1, ObjectCarrier* ObjectCarrier2){
  ObjectCarrier1->setMode(OBJECTCARRIER_MAGAZINE_COLOUR);
  Serial.println("Object carrier MAGAZINE_COLOUR loded");
  ObjectCarrier2->setMode(OBJECTCARRIER_TICTACTOE);
  Serial.println("Object carrier TICTACTOE loaded");
  return 0;
}

int Robot::ticTacToePlace(ObjectCarrier* ObjectCarrier1, ObjectCarrier* ObjectCarrier2, int x2_pos, int y2_pos, int player){
  int type;
  switch(player)
  {
    case PLAYER1:
      type = TYPE_PLAYER1;
      break;
    case PLAYER2:
      type = TYPE_PLAYER2;
      break;
    default:
      Serial.println("Error: unable to place part (Unknown Player)");
      return -1;
  }
  int x1_pos, y1_pos;
  if (ObjectCarrier1->nextOccupiedPosition(x1_pos, y1_pos, type) != 0){
    Serial.println("Error: unable to place part (no parts of required type)");
    return -1;
  }
  int success = movePart(ObjectCarrier1, x1_pos, y1_pos, ObjectCarrier2, x2_pos + TICTACTOE_OFFSET_X, y2_pos + TICTACTOE_OFFSET_Y, 1);
  if(success == 0){
    Serial.println("Successfuly placed part");
  }
  return success;
}

int Robot::manualModeInit(ObjectCarrier* ObjectCarrier1, ObjectCarrier* ObjectCarrier2){
  ObjectCarrier1->setMode(OBJECTCARRIER_MAGAZINE);
  Serial.println("Object carrier MAGAZINE loded");
  ObjectCarrier2->setMode(OBJECTCARRIER_STD);
  Serial.println("Object carrier STANDARD loaded");
  return 0;
}

int Robot::makeFancyDance(ObjectCarrier* ObjectCarrier){
  Serial.println("Making fancy dance");
  MajorAxis->setMovementParameter(STEPPER_SPEED*2, STEPPER_MAXSPEED*2, STEPPER_ACCELERATION*2);
  movePosition(ObjectCarrier, 3, 5, false);
  movePosition(ObjectCarrier, 0, 2, false);
  movePosition(ObjectCarrier, 0, 0, false);
  movePosition(ObjectCarrier, 11, 0,false);
  movePosition(ObjectCarrier, 11, 2,false);
  movePosition(ObjectCarrier, 8, 5, false);
  movePosition(ObjectCarrier, 3, 5, false);
  MajorAxis->setMovementParameter(STEPPER_SPEED, STEPPER_MAXSPEED, STEPPER_ACCELERATION);
  return 0;
}
