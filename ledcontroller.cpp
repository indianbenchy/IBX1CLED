#include "ledcontroller.h"
int RedLed = 0;
int GreenLed = 0;
int BlueLed = 0;
int ColdWhiteLed = 0;
int WarmWhiteLed = 0;
void setLedColor(int redValue, int greenValue, int blueValue, int coldValue, int warmValue) { //Function to change ledstrip color
    if (redValue == RedLed && greenValue == GreenLed && blueValue == BlueLed && coldValue == ColdWhiteLed && warmValue == WarmWhiteLed) {
        return;
    }
   transitionLedColor(redValue,greenValue,blueValue,coldValue,warmValue,100);
}

void changeLedColor(int redValue, int greenValue, int blueValue, int coldValue, int warmValue) { //Function to change ledstrip color
    if (redValue == RedLed && greenValue == GreenLed && blueValue == BlueLed && coldValue == ColdWhiteLed && warmValue == WarmWhiteLed) {
        return;
    }
    int oldR = RedLed;
    int oldG = GreenLed;
    int oldB = BlueLed;
    int oldC = ColdWhiteLed;
    int oldW = WarmWhiteLed;
    transitionLedColor(redValue,greenValue,blueValue,coldValue,warmValue,50);
    transitionLedColor(oldR,oldG,oldB,oldC,oldW,50);
}

void setPins(int redValue, int greenValue, int blueValue, int coldValue, int warmValue) { //Function to change lestrip pins
    pinMode(LED_PIN_R, OUTPUT);
    pinMode(LED_PIN_G, OUTPUT);
    pinMode(LED_PIN_B, OUTPUT);
    pinMode(LED_PIN_W, OUTPUT);
    pinMode(LED_PIN_WW, OUTPUT);

    analogWrite(LED_PIN_R, redValue);
    analogWrite(LED_PIN_G, greenValue);
    analogWrite(LED_PIN_B, blueValue);
    analogWrite(LED_PIN_W, coldValue);
    analogWrite(LED_PIN_WW, warmValue);
}
void transitionLedColor(int endR, int endG, int endB, int endC, int endW, int duration) { //Function to tween color change
    float stepTime = (float)duration / 255.0;
    int rStep = (endR - RedLed) / 255;
    int gStep = (endG - GreenLed) / 255;
    int bStep = (endB - BlueLed) / 255;
    int cStep = (endC - ColdWhiteLed) / 255;
    int wStep = (endW - WarmWhiteLed) / 255;
    
    for (int i = 0; i < 256; i++) {
        int r = RedLed + i * rStep;
        int g = GreenLed + i * gStep;
        int b = BlueLed + i * bStep;
        int c = ColdWhiteLed + i * cStep;
        int w = WarmWhiteLed + i * wStep;
        
        setPins(r, g, b, c, w);
        delay(stepTime);
    }
    RedLed = endR;
    GreenLed = endG;
    BlueLed = endB;
    ColdWhiteLed = endC;
    WarmWhiteLed = endW;
}
void printLEDs(){
  Serial.print("LEDS: ");
  Serial.print(RedLed);
  Serial.print(" ");
  Serial.print(GreenLed);
  Serial.print(" ");
  Serial.print(BlueLed);
  Serial.print(" ");
  Serial.print(ColdWhiteLed);
  Serial.print(" ");
  Serial.print(WarmWhiteLed);
  Serial.print("\n");
}