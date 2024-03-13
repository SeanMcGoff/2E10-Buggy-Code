#pragma once
#ifndef ir_H
#define ir_H
#include <Arduino.h>

namespace IR {

const int LEYE = D12;
const int REYE = D13;
extern bool state[2];

void updateState();

}

#endif