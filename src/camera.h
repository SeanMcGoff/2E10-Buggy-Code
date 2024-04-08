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
  MAXSPEED = 4,
  CHACHA = 5
};  // Defines Movement Tags

const double SPEED_LIMIT_COEFF = 0.85;

extern HUSKYLENS hl;                       // HUSKYLENS Class Instance
extern Camera::TAG mostRecentTag;          // The most recent Tag that the Camera has seen
extern Camera::TAG junctionTurnDirection;  // Which Direction to turn when junction is met
extern bool isSpeedLimited;                // Whether Camera Speed Limit Tag has been Scanne
extern bool slideMode;

void start();                // Starts HUSKYLENS Camera
void updateMostRecentTag();  // Update Tag by pinging camera

}
#endif