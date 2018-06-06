#ifndef OBJECTCARRIER
#define OBJECTCARRIER

#define OBJECTCARRIER_OFFSET_X  110 // x-Offset from coordinat system origin to first element in object carrier (in mm)
#define OBJECTCARRIER_OFFSET_Y  60  // y-Offset from coordinat system origin to first element in object carrier (in mm)
#define OBJECTCARRIER_INCREMENT 20  // Increment between positions in the object carrier in both x and y (in mm)
#define OBJECTCARRIER_NUM_ROWS  6   // Number of maximum rows in Object carrier (y-direction)
#define OBJECTCARRIER_NUM_COLS  12  // Number of maximum colums in Object carrier (x-direction)
#define OBJECTCARRIER_NUM_MAX_TYPES 5 // Number of allowed different types/colours

// for initialisation of positon coordinates
// use with constructor for parameter <position_oc>
#define OBJECTCARRIER_UPPER     0   // relative positon of object carrier on +y side
#define OBJECTCARRIER_LOWER     1   // relative positon of object carrier on -y side

// for initalisation of available positions:
// use with function setMode for parameter <mode>
#define OBJECTCARRIER_STD  0          // standard object carrier; all reachable positions allowed
#define OBJECTCARRIER_TICTACTOE  1    // Object carrier for TicTacToe; 3x3 positions allowed
#define OBJECTCARRIER_MAGAZINE_COLOUR  2  // Magazine Object carrier for two colours; 4x4 for each colour
#define OBJECTCARRIER_MAGAZINE  3     // Magazine Object carrier for one colour; 8x4
#define OBJECTCARRIER_SHAPE_SMILEY  4 //virtual object carrier for shape smiley
#define OBJECTCARRIER_SHAPE_ARROW  5  //virtual object carrier for shape arrow

class ObjPosition
{
  public:
    ObjPosition() : status (0), x(0), y(0) {}
    int status; // position free (0), occupied (1), occupied with colour (2,3,...), reservated for colour (-2,-3,...), locked (-1)
    int x;  // x coordinate (in mm)
    int y;  // y coordinate (in mm)
};

class ObjectCarrier
{
  public:
    ObjectCarrier(int position_oc);  // position_oc describes relative positon of object carrier (0 -> on +y side, 1-> on -y side)
    int setMode(int mode = 0); // sets the mode for the object carrier and initialises positions states accordingly
    int getMode() {return mode;}

    int setPosition(int x_pos, int y_pos, int type = 1);
    int resetPosition(int x_pos, int y_pos, int lockType = 0); // lockType = 0 just resets the positio (-> State = 0), lockType = 1 locks the position for the current colour (-> State =-<CURRENT STATE>)
    int nextFreePosition(int& x_pos, int& y_pos,int type = 1);
    int nextOccupiedPosition(int& x_pos, int& y_pos, int type = 1);
    bool isEmpty();
    bool isFull();

    int getState(int x_pos, int y_pos);
    double getCoordX(int x_pos, int y_pos);
    double getCoordY(int x_pos, int y_pos);

  private:
    int mode;
    ObjPosition Position[OBJECTCARRIER_NUM_COLS][OBJECTCARRIER_NUM_ROWS];
};



#endif
