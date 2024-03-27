#pragma once
#ifndef camera_H
#define camera_H
#include <Arduino.h>
#include <HUSKYLENS.h>
#include <SoftwareSerial.h>
namespace Camera {

enum TAG {
  NOTAG = -1,
  LEFT = 1,
  RIGHT = 2,
  SLOW = 3,
  MAXSPEED = 4
};  // Defines Movement Tags

extern HUSKYLENS hl;  // HUSKYLENS Class Instance
extern Camera::TAG mostRecentTag;

void start();  // Starts HUSKYLENS Camera
void updateMostRecentTag(); // 

}
#endif