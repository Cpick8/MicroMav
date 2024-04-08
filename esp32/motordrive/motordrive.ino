#define fadePin1 5
#define fadePin2 14
#define fadePin3 33
#define fadePin4 15
#define fadePin5 23
#define fadePin6 13

void setup(){
pinMode(fadePin1, OUTPUT);
pinMode(fadePin2, OUTPUT);
pinMode(fadePin3, OUTPUT);
pinMode(fadePin4, OUTPUT);
pinMode(fadePin5, OUTPUT);
pinMode(fadePin6, OUTPUT);
Serial.begin(9600);
}

void loop(){

  for(int i = 0; i<360; i++){
  //convert 0-360 angle to radian (needed for sin function)
  float rad = DEG_TO_RAD * i;
  
  //calculate sin of angle as number between 0 and 255
  int sinOut = constrain((sin(rad) * 128) + 128, 0, 255);
  
  //analogWrite(fadePin1, sinOut);
  analogWrite(fadePin1, 255);
  analogWrite(fadePin2, 255);
  analogWrite(fadePin3, 255);
  analogWrite(fadePin4, 255);
  analogWrite(fadePin5, 255);
  analogWrite(fadePin6, 255);
  
  Serial.println(sinOut);
  delay(15);
  }

}
