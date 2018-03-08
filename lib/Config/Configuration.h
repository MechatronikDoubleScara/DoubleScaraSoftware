/**
 * Double-Scara Robot Firmware
 */

/**
 * Configuration.h
 *
 * Basic settings such as:
 *
 * - Robot geometry
 *
 */

#ifndef CONFIGURATION
#define CONFIGURATION

//===========================================================================
//============================= Geometric Settings ==========================
//===========================================================================

#define BASE_LENGTH 82
#define LINK1_LENGTH 66.7
#define LINK2_LENGTH 105.2

#define ZGEAR_DIAMETER 15
#define Z_MAXLIFT 21

//#define ZAXIS_LIMIT_DOWN 100
//#define ZAXIS_LIMIT_UP 100


//===========================================================================
//============================== Movment Settings ===========================
//===========================================================================



//===========================================================================
//============================ Electrical Settings ==========================
//===========================================================================

#define PIN_ENDEFFECTOR 6
#define PIN_ZAXIS_SERVO 9

#endif
