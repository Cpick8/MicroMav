

#include "PIDconfig.h"

// PID error terms
float pitchError = 0, rollError = 0, yawError = 0, accelerationError_X = 0, accelerationError_Y = 0, accelerationError_Z = 0;
float prevPitchError = 0, prevRollError = 0, prevYawError = 0, prevAccelerationError_X = 0, prevAccelerationError_Y = 0, prevAccelerationError_Z = 0;
float integralPitch = 0, integralRoll = 0, integralYaw = 0, integralAcceleration_X = 0, integralAcceleration_Y = 0, integralAcceleration_Z = 0;

// acceleration variables
float accelerationX = 0, accelerationY = 0, accelerationZ = 0;

void stabilize_(float pitch, float roll, float yaw, float accel[], float* motorSpeeds) {
  #ifdef HEX_X
    // Calculate PID adjustments for pitch, roll, and yaw
    float pitchAdjustment = PIDControl(pitch, prevPitchError, integralPitch, pitchPID);
    float rollAdjustment = PIDControl(roll, prevRollError, integralRoll, rollPID);
    float yawAdjustment = PIDControl(yaw, prevYawError, integralYaw, yawPID);

    // Update previous errors for next iteration
    prevPitchError = pitch;
    prevRollError = roll;
    prevYawError = yaw;

    // Get acceleration values
    float newAccelerationX = accel[0];
    float newAccelerationY = accel[1];
    float newAccelerationZ = accel[2];

    // Calculate acceleration deltas
    float accelerationDeltaX = newAccelerationX - accelerationX;
    float accelerationDeltaY = newAccelerationY - accelerationY;
    float accelerationDeltaZ = newAccelerationZ - accelerationZ;

    // Update previous acceleration values
    accelerationX = newAccelerationX;
    accelerationY = newAccelerationY;
    accelerationZ = newAccelerationZ;

    // Calculate PID adjustments for acceleration in each axis
    float accelerationAdjustment_X = PIDControl(accelerationDeltaX, prevAccelerationError_X, integralAcceleration_X, accelerationPID_X);
    float accelerationAdjustment_Y = PIDControl(accelerationDeltaY, prevAccelerationError_Y, integralAcceleration_Y, accelerationPID_Y);
    float accelerationAdjustment_Z = PIDControl(accelerationDeltaZ, prevAccelerationError_Z, integralAcceleration_Z, accelerationPID_Z);

    // Update previous acceleration errors for next iteration
    prevAccelerationError_X = accelerationDeltaX;
    prevAccelerationError_Y = accelerationDeltaY;
    prevAccelerationError_Z = accelerationDeltaZ;

    // Determine motor adjustments based on pitch, roll, yaw, and acceleration
    float flAdjustment = pitchAdjustment + rollAdjustment - accelerationAdjustment_Y + yawAdjustment;
    float frAdjustment = pitchAdjustment - rollAdjustment + accelerationAdjustment_Y - yawAdjustment;
    float mlAdjustment = pitchAdjustment + rollAdjustment - accelerationAdjustment_X + yawAdjustment;
    float mrAdjustment = pitchAdjustment - rollAdjustment + accelerationAdjustment_X - yawAdjustment;
    float blAdjustment = -pitchAdjustment + rollAdjustment - accelerationAdjustment_Z + yawAdjustment;
    float brAdjustment = -pitchAdjustment - rollAdjustment + accelerationAdjustment_Z - yawAdjustment;

    // Map motor adjustments to the range of 0 to 255
    motorSpeeds[0] = min(255, max(0, static_cast<int>(round(MAX_THROTTLE + flAdjustment))));
    motorSpeeds[1] = min(255, max(0, static_cast<int>(round(MAX_THROTTLE + frAdjustment))));
    motorSpeeds[2] = min(255, max(0, static_cast<int>(round(MAX_THROTTLE + mlAdjustment))));
    motorSpeeds[3] = min(255, max(0, static_cast<int>(round(MAX_THROTTLE + mrAdjustment))));
    motorSpeeds[4] = min(255, max(0, static_cast<int>(round(MAX_THROTTLE + blAdjustment))));
    motorSpeeds[5] = min(255, max(0, static_cast<int>(round(MAX_THROTTLE + brAdjustment))));
  #endif
}


float PIDControl(float currentError, float previousError, float& integral, float pid[]) {
    float proportional = currentError * pid[0]; // P parameter
    integral += currentError * UPDATE_TIME_MS * pid[1]; // I parameter
    float derivative = (currentError - previousError) / UPDATE_TIME_MS * pid[2]; // D parameter
    return proportional + integral + derivative;
}