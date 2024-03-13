#include "ultrasonic.h"

volatile long US::echo_timer;  // Timer for Echo
volatile long US::distance_v;  // Distance Recorded Last by Ultrasonic Sensor


void US::Trigger() {
  digitalWrite(US::TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(US::TRIG, LOW);
  US::echo_timer = micros();
}

void US::Echo_Rising() {
  US::echo_timer = micros();
}

void US::Echo_Falling() {
  if (US::echo_timer != 0) {
    US::distance_v = (micros() - US::echo_timer) / 87;
    //if(distance_i > 1000) distance_i = 0;
    US::echo_timer = 0;
  }
}

long US::getCurrentDistance() {
  long distance;
  ATOMIC() {
    distance = US::distance_v;  // Atomic Block avoids interrupts
  }
  return distance;
}