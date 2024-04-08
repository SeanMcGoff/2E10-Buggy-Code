#include "camera.h"

HUSKYLENS Camera::hl;
Camera::TAG Camera::mostRecentTag = Camera::TAG::NOTAG;         // No tag is default
Camera::TAG Camera::junctionTurnDirection = Camera::TAG::LEFT;  // Left is default
bool Camera::isSpeedLimited = false;                            // Max Speed is Default
bool Camera::slideMode = false;

void Camera::start() {
  Wire.begin();  // Start Wire Connection

  // Check Connection
  while (!Camera::hl.begin(Wire)) {
    Serial.println("HuskyLens is dead.");
    delay(1000);
  }

  // Check Learning of Camera (non-blocking)
  if (!Camera::hl.isLearned()) {
    Serial.println("HuskyLens doesn't know anything.");
  }
}

void Camera::updateMostRecentTag() {
  // If camera sees tag
  if (Camera::hl.request() && Camera::hl.available()) {
    // Read Tag and convert to enum
    HUSKYLENSResult r = Camera::hl.read();
    Camera::mostRecentTag = static_cast<Camera::TAG>(r.ID);
    // Update State Variables based on newest tag
    switch (Camera::mostRecentTag) {
      // Direction Tags
      case Camera::TAG::LEFT:
        Camera::junctionTurnDirection = Camera::TAG::LEFT;
        break;
      case Camera::TAG::RIGHT:
        Camera::junctionTurnDirection = Camera::TAG::RIGHT;
        break;
      // Speed Tags
      case Camera::TAG::SLOW:
        Camera::isSpeedLimited = true;
        break;
      case Camera::TAG::MAXSPEED:
        Camera::isSpeedLimited = false;
        break;
      case Camera::TAG::CHACHA:
        Camera::slideMode = true;
        break;
    }
  }
}