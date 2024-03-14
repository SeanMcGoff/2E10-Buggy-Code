#include "pid.h"

unsigned long PID::currentTime = 0;
unsigned long PID::previousTime = 0;

double PID::error;
double PID::lastError = 0;
double PID::cumError = 0;
double PID::rateError;

int PID::computePID(double distance) {
  PID::currentTime = millis();                                          // Get Current Time
  double elapsedTime = (double)(PID::currentTime - PID::previousTime);  //compute time elapsed from previous computation

  PID::error = (distance - PID::SET_POINT);                      // determine error
  PID::cumError += PID::error * elapsedTime;                     // compute integral
  PID::rateError = (PID::error - PID::lastError) / elapsedTime;  // compute derivative

  int out =
    (PID::KP * PID::error) + (PID::KI * PID::cumError) + (PID::KD * PID::rateError);  //PID output

  out = constrain(out, 0, 255);  // Clamps output between 0 and 255 (8bit unsigned int)

  PID::lastError = PID::error;           //remember current error
  PID::previousTime = PID::currentTime;  //remember current time

  return out;  //have function return the PID output
}