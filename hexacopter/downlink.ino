
void UpdateAP_(float motorSpeed[], float rotation[], float accel[], float temperature) { 
  WiFiClient client = server.available();
  if (client) {
    //Serial.println("New Client.");
    while (client.connected()) {
      if (client.available()) {
        String request = client.readStringUntil('\r');
        //Serial.println(request);
        client.flush();

        // Compile All Telemetry Into JSON
        JSONVar data;
        data["Rotation"]["x"] = rotation[0];
        data["Rotation"]["y"] = rotation[1];
        data["Rotation"]["z"] = rotation[2];
        data["Acceleration"]["x"] = accel[0];
        data["Acceleration"]["y"] = accel[1];
        data["Acceleration"]["z"] = accel[2];
        data["Temperature"] = temperature;

        data["MotorStatus"]["FR"]["Speed"] = motorSpeed[0];
        data["MotorStatus"]["FL"]["Speed"] = motorSpeed[1];
        data["MotorStatus"]["MR"]["Speed"] = motorSpeed[2];
        data["MotorStatus"]["ML"]["Speed"] = motorSpeed[3];
        data["MotorStatus"]["BR"]["Speed"] = motorSpeed[4];
        data["MotorStatus"]["BL"]["Speed"] = motorSpeed[5];
        

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
    //Serial.println("Client disconnected.");
  }
}
