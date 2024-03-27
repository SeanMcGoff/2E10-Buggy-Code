#include "ledmatrix.h"

ArduinoLEDMatrix LED_Matrix::m;

void LED_Matrix::start() {
  LED_Matrix::m.begin();
}

void LED_Matrix::left() {
  LED_Matrix::m.loadFrame(LED_Matrix::LEFT_DESIGN);
}

void LED_Matrix::right() {
  LED_Matrix::m.loadFrame(LED_Matrix::RIGHT_DESIGN);
}  

void LED_Matrix::slow() {
  LED_Matrix::m.loadFrame(LED_Matrix::SLOW_DESIGN);
}  

void LED_Matrix::fast() {
  LED_Matrix::m.loadFrame(LED_Matrix::FAST_DESIGN);
}  
     