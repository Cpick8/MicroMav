#include <Wire.h> // Include the Wire library for I2C communication with the IMU
#include <Adafruit_MPU6050.h>

Adafruit_MPU6050 mpu;


void setup() {
  // Initialize serial communication for debugging
  Serial.begin(9600);

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

  Serial.println("");
  delay(100);

}

void loop() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  // Read IMU data

  float roll = a.acceleration.x;
  float pitch = a.acceleration.y;
  float yaw = a.acceleration.z;
  // Calculate motor speeds based on orientation
  int fl_speed = calculateMotorSpeed(roll, pitch, yaw);
  int fr_speed = calculateMotorSpeed(roll, pitch, yaw);
  int ml_speed = calculateMotorSpeed(roll, pitch, yaw);
  int mr_speed = calculateMotorSpeed(roll, pitch, yaw);
  int bl_speed = calculateMotorSpeed(roll, pitch, yaw);
  int br_speed = calculateMotorSpeed(roll, pitch, yaw);

  // Set motor speeds


  // Print motor speeds for debugging
  Serial.print("FL Speed: ");
  Serial.println(fl_speed);
  
  Serial.print("FR Speed: ");
  Serial.println(fr_speed);
  
  Serial.print("ML Speed: ");
  Serial.println(ml_speed);
  
  Serial.print("MR Speed: ");
  Serial.println(mr_speed);
  
  Serial.print("BL Speed: ");
  Serial.println(bl_speed);
  
  Serial.print("BR Speed: ");
  Serial.println(br_speed);

  delay(100); // Adjust delay as needed
}

// Function to calculate motor speed based on orientation
int calculateMotorSpeed(float roll, float pitch, float yaw) {
  // Add your motor control logic here
  // For a basic example, you could simply map the roll, pitch, and yaw angles to motor speeds
  // Adjust as needed based on your hexacopter's specific requirements
  int motor_speed = map(abs(roll) + abs(pitch) + abs(yaw), 0, 360, 0, 255);
  return motor_speed;
}
