#pragma once
#ifndef ultrasonic_H
#define ultrasonic_H
#include <Arduino.h>
#include "SimplyAtomic.h"

namespace US {

const int ECHO = D2;  // Echo Pin Def
const int TRIG = D3;  // Trigger Pin Def

const int DISTANCE_SENS = 15;  // Distance Sensitivity

extern volatile long echo_timer;    // Timer for Echo
extern volatile double distance_v;  // Distance Recorded Last by Ultrasonic Sensor

void Trigger();               // Send 10µs signal out from Trigger Pin
void Echo_Rising();           // Interrupt Function to be called on rising edge of Echo Pin
void Echo_Falling();          // Interrupt Function to be called on falling edge of Echo Pin
double getCurrentDistance();  // Returns non-volatile distance using atomics

}

#endif