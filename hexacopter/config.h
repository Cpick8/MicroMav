#include <WiFi.h>

//####################################################//
//################### MUST DEFINE! ###################//


// IMU TYPE
#include <Adafruit_MPU6050.h> 
Adafruit_MPU6050 mpu;

// DRONE TYPE
#define HEX_X

// MOTOR PIN OUT
#define MOTOR_FL_PIN 33
#define MOTOR_FR_PIN 23
#define MOTOR_ML_PIN 14
#define MOTOR_MR_PIN 5
#define MOTOR_BL_PIN 13
#define MOTOR_BR_PIN 15

int MAX_THROTTLE = 255; // pwm brushed


// AP CREDS
const char* ssid = "MiniDroneTelemetry";
const char* password = "arduesp32";
WiFiServer server(80);



//####################################################//
//################# SPECIAL FEATURES #################//
bool GPS = false; // dont touch unless you need more preformance without gps

bool TakeOffLevelCalibration = true; // defines offset before takeoff settings below for custom offset (dont touch unless you know what you are doing)
float pitchOffset = 0; //<
float rollOffset = 0; //<
float yawOffset = 0; //<




