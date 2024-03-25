#include "camera.h"

HUSKYLENS Camera::hl;

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