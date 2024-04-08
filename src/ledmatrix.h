#pragma once
#ifndef ledmatrix_H
#define ledmatrix_H
#include <Arduino.h>
#include "Arduino_LED_Matrix.h"

namespace LED_Matrix {

extern ArduinoLEDMatrix m;

const uint32_t LEFT_DESIGN[] = {
  0x800c00,
  0xeffffff0,
  0xe00c008
};

const uint32_t RIGHT_DESIGN[] = {
  0x10030070,
  0xffffff7,
  0x300100
};

const uint32_t SLOW_DESIGN[] = {
  0xfff46240,
  0x22641681,
  0x68090060
};

const uint32_t FAST_DESIGN[] = {
  0xf9f89189,
  0x589d8818,
  0x81891f9f
};

const uint32_t FLASH_DESIGN[] = {
  0xffffffff,
  0xffffffff,
  0xffffffff
};

const uint32_t NO_DESIGN[] = {
  0x00000000,
  0x00000000,
  0x00000000
};



const uint32_t CLAP_DESIGN[] = {
  0x200a80a,
  0xa2aa2fe2,
  0xfe1fe0fc
};

void start();  // Start LED Matrix
void clear();  // Clears LED Matix

void left();   // Display Left Design
void right();  // Display Right Design
void slow();   // Display Slow Design
void fast();   // Display Fast Design

void flash();  // Turn all LEDS on
void clap();   // Clap



}

#endif