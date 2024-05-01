//####################################################//

// Define instances of the PID struct JUST KIDDING STRUCT IS BROKENNNN SO IM DOING IT THE STUPID WAY
float pitchPID[3] = {-5, 0, 0.5};
float rollPID[3] = {5, 0, 0.5};
float yawPID[3] = {0, 0, 0};

float accelerationPID_X[3] = {0, 0, 0};
float accelerationPID_Y[3] = {0, 0, 0};
float accelerationPID_Z[3] = {0, 0, 0};

//####################################################//
//    ######## initilizing other variables ########   //
//####################################################//

//####################################################//

struct rotation_ {
  float Pitch;
  float Roll;
  float Yaw;
};

/*
bool CalibrationSequence = false; // setting to true will negate flight and will run a Calibration to help fill out settings below 
String pitchAxis = "X"; //<\
String rollAxis = "Y"; //<\
String yawAxis = "Z"; //<
*/