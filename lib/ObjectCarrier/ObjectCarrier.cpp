#include <ObjectCarrier.h>

ObjectCarrier::ObjectCarrier(int position_oc){

  int origin_x;
  int origin_y;
  int inc;

  if (position_oc == 0) {
    origin_x = -OBJECTCARRIER_OFFSET_X;
    origin_y = OBJECTCARRIER_OFFSET_Y;
    inc = OBJECTCARRIER_INCREMENT;
  }
  else if (position_oc == 1){
    origin_x = +OBJECTCARRIER_OFFSET_X;
    origin_y = -OBJECTCARRIER_OFFSET_Y;
    inc = -OBJECTCARRIER_INCREMENT;
  }
  else{
    return;
  }

  for (int i=0, y=origin_y; i < OBJECTCARRIER_NUM_ROWS; i++, y += inc){
    for (int j=0, x=origin_x; j < OBJECTCARRIER_NUM_COLS; j++, x += inc){
      Position[j][i].x = x;
      Position[j][i].y = y;
    }
  }
}

int ObjectCarrier::setMode(int type = 0){
  switch (type) {
    case OBJECTCARRIER_STD: // standard object carrier; all reachable positions allowed

      // set every status to 0
      for (int i=0; i < OBJECTCARRIER_NUM_ROWS; i++){
        for (int j=0; j < OBJECTCARRIER_NUM_COLS; j++){
          Position[j][i].status = 0;
        }
      }
      // mark unreachable positions with -1
      Position[0][3].status = -1;
      Position[0][4].status = -1;
      Position[0][5].status = -1;
      Position[1][4].status = -1;
      Position[1][5].status = -1;
      Position[2][5].status = -1;

      Position[11][3].status = -1;
      Position[11][4].status = -1;
      Position[11][5].status = -1;
      Position[10][4].status = -1;
      Position[10][5].status = -1;
      Position[9][5].status = -1;
      break;

    case OBJECTCARRIER_TICTACTOE: // Object carrier for TicTacToe; 3x3 positions allowed (empty)
      for (int i=0; i < OBJECTCARRIER_NUM_ROWS; i++){
        for (int j=0; j < OBJECTCARRIER_NUM_COLS; j++){
          if (j > 2 || i > 2)
            Position[j][i].status = -1; // mark unreachable positions with -1
          else
             Position[j][i].status = 0; // all others with 0
        }
      }
      break;
    case OBJECTCARRIER_MAGAZINE: // Magazine Object carrier for one colour; 8x4 (full)
      for (int i=0; i < OBJECTCARRIER_NUM_ROWS; i++){
        for (int j=0; j < OBJECTCARRIER_NUM_COLS; j++){
          if (j > 1 && j <= 5 && i > 0 && i <= 4 )
            Position[j][i].status = 1; // positon std type
          else if (j > 5 && j <= 9 && i > 0 && i <= 4 )
            Position[j][i].status = 1; // positon std type
          else
            Position[j][i].status = -1; // mark unreachable positions with -1
        }
      }
      break;

    case OBJECTCARRIER_MAGAZINE_COLOUR: // Magazine Object carrier for two colours; 4x4 for each colour (full)
      for (int i=0; i < OBJECTCARRIER_NUM_ROWS; i++){
        for (int j=0; j < OBJECTCARRIER_NUM_COLS; j++){
          if (j > 1 && j <= 5 && i > 0 && i <= 4 )
            Position[j][i].status = 2; // positon colour 2
          else if (j > 5 && j <= 9 && i > 0 && i <= 4 )
            Position[j][i].status = 3; // positon colour 3
          else
            Position[j][i].status = -1; // mark unreachable positions with -1
        }
      }
      break;
    case OBJECTCARRIER_SHAPE_SMILEY:
      // set every status to -1
      for (int i=0; i < OBJECTCARRIER_NUM_ROWS; i++){
        for (int j=0; j < OBJECTCARRIER_NUM_COLS; j++){
          Position[j][i].status = -1;
        }
      }
      // mark positions for shape with 0
      Position[3][1].status = 0;
      Position[4][1].status = 0;
      Position[5][1].status = 0;
      Position[6][1].status = 0;
      Position[2][2].status = 0;
      Position[7][2].status = 0;
      Position[3][4].status = 0;
      Position[6][4].status = 0;
      break;

    case OBJECTCARRIER_SHAPE_ARROW:
      // set every status to -1
      for (int i=0; i < OBJECTCARRIER_NUM_ROWS; i++){
        for (int j=0; j < OBJECTCARRIER_NUM_COLS; j++){
          Position[j][i].status = -1;
        }
      }
      // mark positions for shape with 0
      Position[3][3].status = 0;
      Position[4][3].status = 0;
      Position[5][3].status = 0;
      Position[6][3].status = 0;
      Position[7][3].status = 0;
      Position[8][3].status = 0;
      Position[7][2].status = 0;
      Position[6][1].status = 0;
      Position[7][4].status = 0;
      Position[6][5].status = 0;
      break;

    default: //no valid mode
      return -1;
  }
}

int ObjectCarrier::setPosition(int x_pos,int y_pos,int type = 1){
  //check for errors
  if (x_pos < 0 || x_pos >= OBJECTCARRIER_NUM_COLS || y_pos < 0 || y_pos >= OBJECTCARRIER_NUM_ROWS)
    return -1; // position does not exist
  else if (Position[x_pos][y_pos].status == -1)
    return -2; // position not allowed
  else if (Position[x_pos][y_pos].status == type)
    return -3; // position not free
  else if (Position[x_pos][y_pos].status != (-type) && Position[x_pos][y_pos].status != 0)
    return  -4; // wrong type for position

  Position[x_pos][y_pos].status = type;
  return 0;
}

int ObjectCarrier::resetPosition(int x_pos,int y_pos,int lockType = 0){
  //check for errors
  if (x_pos < 0 || x_pos >= OBJECTCARRIER_NUM_COLS || y_pos < 0 || y_pos >= OBJECTCARRIER_NUM_ROWS)
    return -1; // position does not exsist
  else if (Position[x_pos][y_pos].status == -1)
    return -2; // position not allowed
  else if (Position[x_pos][y_pos].status <= 0)
    return -3; // position is free, nothing to remove


  if (lockType == 1 && Position[x_pos][y_pos].status != 1){ //standard type 1 can not be locked
    Position[x_pos][y_pos].status = -(Position[x_pos][y_pos].status);
  }
  else{
    Position[x_pos][y_pos].status = 0;
  }
  return 0;
}


int ObjectCarrier::nextFreePosition(int& x_pos, int& y_pos, int type = 1){
  if (type == 1) type = 0; // we are searching for empty positions without restrictions
  else type = -(type); // we are searching for an empty positions for a specific colour

  for (int i=0; i < OBJECTCARRIER_NUM_ROWS; i++){
    for (int j=0; j < OBJECTCARRIER_NUM_COLS; j++){
      if (Position[j][i].status == type){
        x_pos = j;
        y_pos = i;
        return 0;
      }
    }
  }
  return -1;  //no suitable position left
}

int ObjectCarrier::nextOccupiedPosition(int& x_pos, int& y_pos, int type = 1){
  for (int i=0; i < OBJECTCARRIER_NUM_ROWS; i++){
    for (int j=0; j < OBJECTCARRIER_NUM_COLS; j++){
      if (Position[j][i].status == type){
        x_pos = j;
        y_pos = i;
        return 0;
      }
    }
  }
  return -1;  //no occupied position of type <type>
}


bool ObjectCarrier::isEmpty(){
  for (int i=0; i < OBJECTCARRIER_NUM_ROWS; i++){
    for (int j=0; j < OBJECTCARRIER_NUM_COLS; j++){
      if (Position[j][i].status > 0){
        return false; // still objects left
      }
    }
  }
  return true; // object carrier is empty
}

bool ObjectCarrier::isFull(){
  for (int i=0; i < OBJECTCARRIER_NUM_ROWS; i++){
    for (int j=0; j < OBJECTCARRIER_NUM_COLS; j++){
      if (Position[j][i].status <= 0 && Position[j][i].status != -1){
        return false; // still empty positions left
      }
    }
  }
  return true; // object carrier is full
}

int ObjectCarrier::getState(int x_pos, int y_pos){
  if (x_pos < 0 || x_pos >= OBJECTCARRIER_NUM_COLS || y_pos < 0 || y_pos >= OBJECTCARRIER_NUM_ROWS)
    return -1; // position does not exist
  else
    return Position[x_pos][y_pos].status;
}

double ObjectCarrier::getCoordX(int x_pos, int y_pos){
  if (x_pos < 0 || x_pos >= OBJECTCARRIER_NUM_COLS || y_pos < 0 || y_pos >= OBJECTCARRIER_NUM_ROWS)
    return 0.0; // position does not exist
  else
    return Position[x_pos][y_pos].x;
}

double ObjectCarrier::getCoordY(int x_pos, int y_pos){
  if (x_pos < 0 || x_pos >= OBJECTCARRIER_NUM_COLS || y_pos < 0 || y_pos >= OBJECTCARRIER_NUM_ROWS)
    return 0.0; // position does not exist
  else
    return Position[x_pos][y_pos].y;
}
