#pragma once
#ifndef ir_H
#define ir_H
#include <Arduino.h>

namespace IR {

const int LEYE = D12;  // Left IR Sensor Pin
const int REYE = D13;  // Right IR Sensor Pin

extern bool state[2];  // Current IR State (defined in ir.cpp)

void updateState();  // update IR::state

}

#endif