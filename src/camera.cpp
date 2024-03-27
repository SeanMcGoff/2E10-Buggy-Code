#include "camera.h"

HUSKYLENS Camera::hl;
Camera::TAG Camera::mostRecentTag = Camera::TAG::NOTAG;

void Camera::start() {
  Wire.begin();	// Start Wire Connection

  // Check Connection
  while( !Camera::hl.begin(Wire) ){	
    Serial.println("HuskyLens is dead.");	
    delay(1000);	
  }

  // Check Learning of Camera (non-blocking)
  if( !Camera::hl.isLearned() ){	
    Serial.println("HuskyLens doesn't know anything.");	
  }
}

void Camera::updateMostRecentTag() {
  if(Camera::hl.request() && Camera::hl.available()) {
    HUSKYLENSResult r = Camera::hl.read();
    Camera::mostRecentTag = static_cast<Camera::TAG>(r.ID);
  }
}