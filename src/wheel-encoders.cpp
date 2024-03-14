#include "wheel-encoders.h"

bool WheelEncoders::state[2] = { true, true };
double WheelEncoders::distanceTraveled;

void WheelEncoders::update() {

  // Get Current State
  bool leftState = digitalRead(WheelEncoders::LEFT);
  bool rightState = digitalRead(WheelEncoders::RIGHT);

  // If Left State Changed...
  if (WheelEncoders::state[0] != leftState) {
    WheelEncoders::state[0] = leftState;
    WheelEncoders::distanceTraveled += WheelEncoders::CIRCUMFRENCE / 8;
  }

  // If Right State Changed...
  if (WheelEncoders::state[1] != rightState) {
    WheelEncoders::state[1] = rightState;
    WheelEncoders::distanceTraveled += WheelEncoders::CIRCUMFRENCE / 8;
  }
}