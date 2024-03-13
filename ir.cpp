#include "ir.h"

bool IR::state[2];

void IR::updateState() {
  IR::state[0] = digitalRead(IR::LEYE);
  IR::state[1] = digitalRead(IR::REYE);
}