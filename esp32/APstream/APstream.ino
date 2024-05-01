#include <WiFi.h>
#include <Arduino_JSON.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

#define MOTOR_FL_PIN 33
#define MOTOR_FR_PIN 23
#define MOTOR_ML_PIN 14
#define MOTOR_MR_PIN 5
#define MOTOR_RL_PIN 13
#define MOTOR_RR_PIN 15

#define MAX_throttle = 255

#define FL_Speed = 0
#define FR_Speed = 0
#define ML_Speed = 0
#define MR_Speed = 0
#define BL_Speed = 0
#define BR_Speed = 0


Adafruit_MPU6050 mpu;

// AP credentials
const char* ssid = "MiniDroneTelemetry";
const char* password = "arduesp32";
WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  delay(2000);

  Serial.println("=========== LAUNCHING AP ===========");
  Serial.print("SSID: ");
  Serial.println(ssid);
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("IP: ");
  Serial.println(IP);
  server.begin();
  Serial.println("=========== ++++++++++++ ===========");

  //sample data 
  Serial.println("Adafruit MPU6050 test!");

  // Try to initialize!
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

  /* Print out the values */
  Serial.print("Acceleration X: ");
  Serial.print(a.acceleration.x);
  Serial.print(", Y: ");
  Serial.print(a.acceleration.y);
  Serial.print(", Z: ");
  Serial.print(a.acceleration.z);
  Serial.println(" m/s^2");

  Serial.print("Rotation X: ");
  Serial.print(g.gyro.x);
  Serial.print(", Y: ");
  Serial.print(g.gyro.y);
  Serial.print(", Z: ");
  Serial.print(g.gyro.z);
  Serial.println(" rad/s");

  Serial.print("Temperature: ");
  Serial.print(temp.temperature);
  Serial.println(" degC");

  Serial.println("");

  
  WiFiClient client = server.available();
  if (client) {
    Serial.println("New Client.");
    while (client.connected()) {
      if (client.available()) {
        String request = client.readStringUntil('\r');
        Serial.println(request);
        client.flush();

        // Sample data to send
        JSONVar data;
        data["Rotation"]["x"] = a.acceleration.x;
        data["Rotation"]["y"] = a.acceleration.y;
        data["Rotation"]["z"] = a.acceleration.z;
        data["Acceleration"]["x"] = g.gyro.x;
        data["Acceleration"]["y"] = g.gyro.y;
        data["Acceleration"]["z"] = g.gyro.z;
        data["Temperature"] = temp.temperature;

        float roll = a.acceleration.x;
        float pitch = a.acceleration.y;
        float yaw = a.acceleration.z;

        data["MotorStatus"]["FR"]["Speed"] = calculateMotorSpeed(roll, pitch, yaw);
        data["MotorStatus"]["FL"]["Speed"] = calculateMotorSpeed(roll, pitch, yaw);
        data["MotorStatus"]["MR"]["Speed"] = calculateMotorSpeed(roll, pitch, yaw);
        data["MotorStatus"]["ML"]["Speed"] = calculateMotorSpeed(roll, pitch, yaw);
        data["MotorStatus"]["BR"]["Speed"] = calculateMotorSpeed(roll, pitch, yaw);
        data["MotorStatus"]["BL"]["Speed"] = calculateMotorSpeed(roll, pitch, yaw);
        

        // Convert JSON to string
        String jsonData = JSON.stringify(data);

        // Send JSON data to the client
        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: application/json");
        client.println("Connection: close");
        client.println();
        client.println(jsonData);

        break;
      }
    }
    client.stop();
    Serial.println("Client disconnected.");
  }
}


int calculateMotorSpeed(float roll, float pitch, float yaw) {
  // Add your motor control logic here
  // For a basic example, you could simply map the roll, pitch, and yaw angles to motor speeds
  // Adjust as needed based on your hexacopter's specific requirements
  int motor_speed = map(abs(roll) + abs(pitch) + abs(yaw), 0, 360, 0, 255);
  return motor_speed;
}