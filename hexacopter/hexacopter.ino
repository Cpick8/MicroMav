#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Arduino_JSON.h>

#include "config.h"

int UPDATE_TIME_MS = 200; // milliseconds 200 = 5hz

/* TODO
- Add start offset X
- throttle adjustment
- takeoff mode
*/


// main loop! :)
void loop() {
  // get state info
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  float rotation[] = {a.acceleration.x - pitchOffset, a.acceleration.y - rollOffset, a.acceleration.z - yawOffset};
  // realtime adjustments};
  float accel[] = {g.gyro.x, g.gyro.y, g.gyro.z};
  float temperature = temp.temperature;

  float pitch = rotation[0];
  float roll = rotation[1];
  float yaw = rotation[2];
  // realtime adjustments
  float motorSpeeds[6];
  stabilize_(pitch, roll, yaw, accel, motorSpeeds);


  #ifdef HEX_X
    analogWrite(MOTOR_FR_PIN, motorSpeeds[0]);
    analogWrite(MOTOR_FL_PIN, motorSpeeds[1]);
    analogWrite(MOTOR_MR_PIN, motorSpeeds[2]);
    analogWrite(MOTOR_ML_PIN, motorSpeeds[3]);
    analogWrite(MOTOR_BR_PIN, motorSpeeds[4]);
    analogWrite(MOTOR_BL_PIN, motorSpeeds[5]);
  #endif

  // send packet of new state
  UpdateAP_(motorSpeeds, rotation, accel, temperature);
  delay(UPDATE_TIME_MS);
}








//####################################################//
//####################### INIT #######################//
//####################################################//

// kickstart ap, find IMU on rx and tx, and define all motors
void setup() {
  Serial.begin(115200);
  #ifdef HEX_X
    pinMode(MOTOR_FL_PIN, OUTPUT);
    pinMode(MOTOR_FR_PIN, OUTPUT);
    pinMode(MOTOR_ML_PIN, OUTPUT);
    pinMode(MOTOR_MR_PIN, OUTPUT);
    pinMode(MOTOR_BL_PIN, OUTPUT);
    pinMode(MOTOR_BR_PIN, OUTPUT);
  #endif
  delay(2000);

  // AP SETUP
  Serial.println("=========== LAUNCHING AP ===========");
  Serial.print("SSID: ");
  Serial.println(ssid);
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("IP: ");
  Serial.println(IP);
  server.begin();
  Serial.println("=========== ++++++++++++ ===========");

  //IMU SETUP
  Serial.println("Searching for Adafruit MPU6050");

  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");
  
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
  case MPU6050_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case MPU6050_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case MPU6050_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case MPU6050_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
  case MPU6050_RANGE_250_DEG:
    Serial.println("+- 250 deg/s");
    break;
  case MPU6050_RANGE_500_DEG:
    Serial.println("+- 500 deg/s");
    break;
  case MPU6050_RANGE_1000_DEG:
    Serial.println("+- 1000 deg/s");
    break;
  case MPU6050_RANGE_2000_DEG:
    Serial.println("+- 2000 deg/s");
    break;
  }

  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
  case MPU6050_BAND_260_HZ:
    Serial.println("260 Hz");
    break;
  case MPU6050_BAND_184_HZ:
    Serial.println("184 Hz");
    break;
  case MPU6050_BAND_94_HZ:
    Serial.println("94 Hz");
    break;
  case MPU6050_BAND_44_HZ:
    Serial.println("44 Hz");
    break;
  case MPU6050_BAND_21_HZ:
    Serial.println("21 Hz");
    break;
  case MPU6050_BAND_10_HZ:
    Serial.println("10 Hz");
    break;
  case MPU6050_BAND_5_HZ:
    Serial.println("5 Hz");
    break;
  }
  Serial.println("=========== ++++++++++++ ===========");
  Serial.println("========== SETUP COMPLETE ==========");
  Serial.println("=========== ++++++++++++ ===========");
  delay(1000); 
  if (TakeOffLevelCalibration == true) {
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
    pitchOffset = a.acceleration.x; 
    rollOffset = a.acceleration.y; 
    yawOffset = a.acceleration.z;
  }
  delay(100); // time before loop
}