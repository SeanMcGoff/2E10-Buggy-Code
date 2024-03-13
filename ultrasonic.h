#pragma once
#ifndef ultrasonic_H
#define ultrasonic_H
#include <Arduino.h>
#include "SimplyAtomic.h"

namespace US {

const int ECHO = D2;  // Echo Pin Def
const int TRIG = D3;  // Trigger Pin Def

const int DISTANCE_SENS = 15;  // Distance Sensitivity

extern volatile long echo_timer;  // Timer for Echo
extern volatile long distance_v;  // Distance Recorded Last by Ultrasonic Sensor

void Trigger();
void Echo_Rising();
void Echo_Falling();
long getCurrentDistance();

}

#endif