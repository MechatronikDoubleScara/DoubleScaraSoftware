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
#define NUMBER_EIGHTH_STEPS 1600

#define INITIAL_Y_POSITION 100

//#define ZAXIS_LIMIT_DOWN 100
//#define ZAXIS_LIMIT_UP 100

//===========================================================================
//============================== Movement Settings ===========================
//===========================================================================

#define Z_SERVO_SPEED 80 // defines the speed of the servo motor for z lift (0-255)
#define Z_SERVO_POS_UP 0.0 // defines default up and down position for move Up/Down instructions in millimeters (0-21,5)
#define Z_SERVO_POS_DOWN 18
#define Z_SERVO_CORRECTION_FACTOR 1.113 //empirical correction factor for conversion to linear movement

#define SENSOR1_OFFSET 30.14 //32.25 // -147.75 + 180
#define SENSOR2_OFFSET 66.17 //65.05

#define STEPPER_MAXSPEED 3000
#define STEPPER_SPEED 3000
#define STEPPER_ACCELERATION 1500

//===========================================================================
//============================== Bluetooth Settings ===========================
//===========================================================================

#define MAX_STRING_ENTRIES 20 // defines the maximum amount of string messages, that can be received over bluetooth

//===========================================================================
//============================ Electrical Settings ==========================
//===========================================================================

#define PIN_SOFTWARESERIAL_RX A14 //TX am Bluetooth
#define PIN_SOFTWARESERIAL_TX 17 //RX am Bluetooth
#define PIN_ENDEFFECTOR 45
#define PIN_ZAXIS_SERVO 13
#define PIN_SS_SENSOR1 12
#define PIN_SS_SENSOR2 11

//Stepper Driver Pins -> Richtige eintragen
#define PIN_SMD_ENABLE 14
#define PIN_MOTOR1_STEPS 7
#define PIN_MOTOR2_STEPS 5
#define PIN_MOTOR1_DIR 8
#define PIN_MOTOR2_DIR 6

#endif
