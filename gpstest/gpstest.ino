#include "TinyGPS++.h"
#include "SoftwareSerial.h"

SoftwareSerial serial_connection(10,11);
TinyGPSPlus gps;
void setup() {
  Serial.begin(9600);
  serial_connection.begin(9600);
  Serial.println("GPS Start");  
}

void loop() {
  while(serial_connection.available()) 
  {
    gps.encode(serial_connection.read());
  }
  if(gps.location.isUpdated())
  {

    Serial.println("SAT COUNT:");
    Serial.println(gps.satellites.value());
    Serial.println("lat");
    Serial.println(gps.location.lat(), 6);
    Serial.println("lon");
    Serial.println(gps.location.lng(), 6);
    Serial.println("MPH");
    Serial.println(gps.speed.mph());
    Serial.println("Alt");
    Serial.println(gps.altitude.feet());
    Serial.println("");
    
  }
}
