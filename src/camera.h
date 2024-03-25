#pragma once
#ifndef camera_H
#define camera_H
#include <Arduino.h>
#include <HUSKYLENS.h>
#include <SoftwareSerial.h>
namespace Camera {

enum TAGS {
  LEFT = 1,
  RIGHT = 2,
  SLOW = 3,
  MAXSPEED = 4
};

extern HUSKYLENS hl;


void start();  // Starts HUSKYLENS Camera

}
#endif