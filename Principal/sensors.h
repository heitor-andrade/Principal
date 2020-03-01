#ifndef SENSORS_H
#define SENSORS_H

#include "constants.h"
#include "control.h"
#include "Button.h"
#include "pins.h"

Button my_button(BUTTON_PIN);

void calibrateSensors();
void resetCalibration();

float readArray();
bool readRight();
bool readLeft();

long ledEsquerdoAceso = millis();
long ledDireitoAceso = millis();

bool mapeando = true;

#include "QTRSensors.h"
#include "Constants.h"
#include <Arduino.h>
#include "Pins.h"

unsigned int arraySensorsValue[8];

QTRSensorsAnalog frontalSensors((unsigned char[]){
  55, 56, 57, 58, 59, 60, 61, 62, 63, 64
}, 8, 1/*, 255 */);   // 4(numSamples) and 255(emitterPin) are default     

QTRSensorsAnalog rightSensor((unsigned char[]){
  
  54
}, 1, 1);

QTRSensorsAnalog leftleftSensor((unsigned char[]){
  65
}, 1, 1);

QTRSensorsAnalog leftrightSensor((unsigned char[]){
  66
}, 1, 1);

void calibrateSensors()
{
  digitalWrite(LED_PIN, LOW);
  
  frontalSensors.calibrate();
  rightSensor.calibrate();
  leftleftSensor.calibrate();
  leftrightSensor.calibrate();

  digitalWrite(LED_PIN, HIGH);
}

void resetCalibration()
{
  frontalSensors.resetCalibration();
  rightSensor.resetCalibration();
  leftleftSensor.resetCalibration();
  leftrightSensor.resetCalibration();
}

float readArray()
{
  int line = frontalSensors.readLine(arraySensorsValue, QTR_EMITTERS_ON, USE_WHITE_LINE);
  float aux = (ARRAY_CENTER_POSITION - line)/1000.0;

  return aux; //invertido (ver followPath)
}

bool readRight()
{
  unsigned int rightValue;
  rightSensor.readCalibrated(&rightValue);
  
  if ( rightValue < LINE_VALUE){
    digitalWrite(LED_RIGHT, HIGH);
    ledDireitoAceso = millis();
  }
  if (millis() - ledDireitoAceso > 100)
    digitalWrite(LED_RIGHT, LOW);
  
  
  return rightValue < LINE_VALUE;
}

bool readLeft()
{
  unsigned int leftleftValue;
  leftleftSensor.readCalibrated(&leftleftValue);
  unsigned int leftrightValue;
  leftrightSensor.readCalibrated(&leftrightValue);

  if ((leftleftValue < LINE_VALUE) or (leftrightValue < LINE_VALUE)){
    digitalWrite(LED_LEFT, HIGH);
    ledEsquerdoAceso = millis();
  }
  if (millis() - ledEsquerdoAceso > 100)
    digitalWrite(LED_LEFT, LOW);
  
  return ((leftleftValue < LINE_VALUE) or (leftrightValue < LINE_VALUE));
}

bool buttonPressed() {
  return my_button.isPressed();
}

void updateIRs() {
  main_loop.input = readArray();
}

bool detectMark(bool side) { // marcacoes pros 2 lados
  return readRight();
}


#endif
