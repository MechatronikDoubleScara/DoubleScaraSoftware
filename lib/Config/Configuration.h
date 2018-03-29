#ifndef CONFIGURATION
#define CONFIGURATION

//===========================================================================
//============================= Geometric Settings ==========================
//===========================================================================

#define BASE_LENGTH 82
#define LINK1_LENGTH 72.1 // 66.7 oder 72.1
#define LINK2_LENGTH 113.1 // 105.2 oder 113.1
#define NEMA 42

#define ZGEAR_DIAMETER 15
#define Z_MAXLIFT 21

#define NUMBER_SINGLE_STEPS 200
#define NUMBER_MICRO_STEPS 51200

//#define ZAXIS_LIMIT_DOWN 100
//#define ZAXIS_LIMIT_UP 100

//===========================================================================
//============================== Movement Settings ===========================
//===========================================================================

#define Z_SERVO_SPEED 50 // defines the speed of the servo motor for z lift (0-255)
#define SENSOR1_OFFSET 0
#define SENSOR2_OFFSET 0

#define STEPPER_MAXSPEED 3000
#define STEPPER_ACCELERATION 2500

#define INIT_ANGLE_LINK 30

//===========================================================================
//============================== Bluetooth Settings ===========================
//===========================================================================

#define MAX_STRING_ENTRIES 20 // defines the maximum amount of string messages, that can be received over bluetooth

//===========================================================================
//============================ Electrical Settings ==========================
//===========================================================================

#define PIN_SOFTWARESERIAL_RX 2
#define PIN_SOFTWARESERIAL_TX 3
#define PIN_ENDEFFECTOR 6
#define PIN_ZAXIS_SERVO 8
#define PIN_SS_SENSOR1 10
#define PIN_SS_SENSOR2 9

#endif
