#ifndef ROBOT
#define ROBOT

#include <math.h>
#include <Endeffector.h>
#include <ZAxis.h>
#include <MajorAxis.h>
#include <ObjectCarrier.h>
#include <Configuration.h>

// Defines for Modes Shapes, ticTacToe for external function calls
#define SHAPE_SMILEY 1
#define SHAPE_ARROW 2

#define PLAYER1 1
#define PLAYER2 2
#define TYPE_PLAYER1 2
#define TYPE_PLAYER2 3

#define TICTACTOE_OFFSET_X 3
#define TICTACTOE_OFFSET_Y 3



class Robot{
public:
  Robot(MajorAxis* MA, ZAxis* ZA, Endeffector* EF) : MajorAxis(MA), ZAxis(ZA), Endeffector(EF) {}

  int movePart(ObjectCarrier* ObjectCarrier1, int x1_pos, int y1_pos, ObjectCarrier* ObjectCarrier2, int x2_pos, int y2_pos, int lockType=0);
  int movePosition(ObjectCarrier* ObjectCarrier, int x_pos, int y_pos, bool correction = true);
  //int moveManual(double x_coord, double y_coord, double z_coord); //TODO:integrate manual move
  int cleanPlate(ObjectCarrier* ObjectCarrier1, ObjectCarrier* ObjectCarrier2); //clean ObjectCarrier1 and store elements in ObjectCarrier2
  int fillPlate(ObjectCarrier* ObjectCarrier1, ObjectCarrier* ObjectCarrier2); // fill ObjectCarrier2 with suitable Elements of Type of Object Carrier1
  int makeShape(ObjectCarrier* ObjectCarrier1, ObjectCarrier* ObjectCarrier2, int shape);
  int ticTacToeInit(ObjectCarrier* ObjectCarrier1, ObjectCarrier* ObjectCarrier2);
  int ticTacToePlace(ObjectCarrier* ObjectCarrier1, ObjectCarrier* ObjectCarrier2, int x2_pos, int y2_pos, int player);
  int manualModeInit(ObjectCarrier* ObjectCarrier1, ObjectCarrier* ObjectCarrier2);
  int makeFancyDance(ObjectCarrier* ObjectCarrier);
  int goHome();


private:
  MajorAxis* MajorAxis;
  ZAxis* ZAxis;
  Endeffector* Endeffector;
};


#endif
