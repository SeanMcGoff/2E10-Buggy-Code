#pragma once
#ifndef pid_H
#define pid_H
#include <Arduino.h>
#include <PID_v1.h>

namespace Pid {

//PID constants
const double KP = 16;  // Proportional Constant
const double KI = 0;   // Integral Constant
const double KD = 0;   // Differential Constant

const double ENABLING_DISTANCE = 50;  // When PID Enables according to buggy

extern PID buggyPID;
extern double Input, Output, SetPoint;
extern bool enabled;

int computePID(double distance);  // computes new PID and updates relevant variables

}

#endif