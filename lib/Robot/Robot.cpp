#include <Robot.h>

int Robot::movePart(ObjectCarrier* ObjectCarrier1, int x1_pos, int y1_pos, ObjectCarrier* ObjectCarrier2, int x2_pos, int y2_pos, int lockType=0){

  float x1_coord = ObjectCarrier1->getCoordX(x1_pos, y1_pos);
  float y1_coord = ObjectCarrier1->getCoordY(x1_pos, y1_pos);
  int type = ObjectCarrier1->getState(x1_pos, y1_pos);

  float x2_coord = ObjectCarrier2->getCoordX(x2_pos, y2_pos);
  float y2_coord = ObjectCarrier2->getCoordY(x2_pos, y2_pos);


  if (abs(x1_coord) > 1 || abs(y1_coord) > 1 || abs(x2_coord) > 1 || abs(y2_coord) > 1)
    return -1;

  MajorAxis->movePosition(x1_coord, y1_coord);
  ZAxis->moveDown();
  delay(500);
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
        movePart(ObjectCarrier1, x1_pos, y1_pos, ObjectCarrier2, x2_pos, y2_pos, 1);
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
  for (int type = 1; (!ObjectCarrier1->isFull()) && (type <= OBJECTCARRIER_NUM_MAX_TYPES); type++){
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
  if (ObjectCarrier1->isFull()){
    return 0;
  }
  else{
    Serial.print("Error: unable to fill plate (unknown error)\n");
    return-1;
  }
}
