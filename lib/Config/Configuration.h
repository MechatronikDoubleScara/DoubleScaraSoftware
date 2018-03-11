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
//============================== Movement Settings ===========================
//===========================================================================


//===========================================================================
//============================== Bluetooth Settings ===========================
//===========================================================================

#define PIN_SOFTWARESERIAL_RX 2
#define PIN_SOFTWARESERIAL_TX 3
#define MAX_STRING_ENTRIES 20 // defines the maximum amount of string messages, that can be received over bluetooth


//===========================================================================
//============================ Electrical Settings ==========================
//===========================================================================

#define PIN_ENDEFFECTOR 6
#define PIN_ZAXIS_SERVO 9

#endif
