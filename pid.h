#pragma once
#ifndef pid_H
#define pid_H
#include <Arduino.h>

namespace PID {

//PID constants
const double KP = 30;  // Proportional Constant
const double KI = 0;   // Integral Constant
const double KD = 50;  // Differential Constant

const double SET_POINT = 15;  // Target Distance (in cm) away from object

extern unsigned long currentTime, previousTime;       // Time Variables
extern double error, lastError, cumError, rateError;  // Error Variables

int computePID(double distance);

}

#endif