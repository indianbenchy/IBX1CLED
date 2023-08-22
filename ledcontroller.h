#ifndef LED_CONTROLLER_H
#define LED_CONTROLLER_H
#include <Arduino.h>


#define LED_PIN_R D1  // Red pin
#define LED_PIN_G D3 // Green pin
#define LED_PIN_B D2  // Blue pin
#define LED_PIN_W D5  // White pin
#define LED_PIN_WW D6 // Warm white pin




void setPins(int redValue, int greenValue, int blueValue, int coldValue, int warmValue);
void setLedColor(int redValue, int greenValue, int blueValue, int coldValue, int warmValue);
void changeLedColor(int redValue, int greenValue, int blueValue, int coldValue, int warmValue);
void setPins(int redValue, int greenValue, int blueValue, int coldValue, int warmValue);
void transitionLedColor(int endR, int endG, int endB, int endC, int endW, int duration);
void printLEDs();

#endif