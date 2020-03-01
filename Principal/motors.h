#include <arduino.h>
#include "sensors.h"
#include "bluetooth.h"
#include "track.h"
#include "encoder.h"
#include "mapeamento.h"

#ifndef MOTORS_H
#define MOTORS_H

int switchALigado = 0;
int switchBLigado = 0;
int switchCLigado = 0;
int switchDLigado = 0;

void setPWMs(int left, int right) {
  // Inverte sinais
  left = -left;
  right = -right;

  // Garantindo que a velocidade angular é atingida
  int difPos = max(left, right) - 255;
  if (difPos > 0) {
    left -= difPos;
    right -= difPos;
  }
  int difNeg = min(left, right) + 255;
  if (difNeg < 0) {
    left -= difNeg;
    right -= difNeg;
  }

  if (left > 0) {
    analogWrite(L_MOTOR_2, left);
    digitalWrite(L_MOTOR_1, 0);
  }
  else {
    digitalWrite(L_MOTOR_2, 0);
    analogWrite(L_MOTOR_1, abs(left));
  }

  if (right > 0) {
    digitalWrite(R_MOTOR_2, 0);
    analogWrite(R_MOTOR_1, right);
  }
  else {
    analogWrite(R_MOTOR_2, abs(right));
    digitalWrite(R_MOTOR_1, 0);
  }
}

void stopRobot() {
  digitalWrite(L_MOTOR_2, 0);
  digitalWrite(L_MOTOR_1, 0);
  digitalWrite(R_MOTOR_2, 0);
  digitalWrite(R_MOTOR_1, 0);
}

void moveSimple() {
  int left;
  int right;
  float sensorRead;

    if (!mapeando){
      left = constrain(configs.PWM * (meusMultiplicadores[mappingPath] + (((contadorEncoderDireito + contadorEncoderEsquerdo)/2)/comprimentoTotal[mappingPath-1])) - main_loop.output, -255, 255);
      right = constrain(configs.PWM * (meusMultiplicadores[mappingPath] + (((contadorEncoderDireito + contadorEncoderEsquerdo)/2)/comprimentoTotal[mappingPath-1])) + main_loop.output, -255, 255);
    }
    /*else if (!mapeamento) {
      left = constrain(configs.PWM * meusMultiplicadores[mappingPath] + main_loop.output, -255, 255);
      right = constrain(configs.PWM * meusMultiplicadores[mappingPath] - main_loop.output, -255, 255);
    }*/
    else {
      left = constrain(configs.PWM + main_loop.output, -255, 255);
      right = constrain(configs.PWM - main_loop.output, -255, 255);
    }

    if (jaFreiou){
      //digitalWrite(LED_LEFT, HIGH);
      left = configs.PWM * 1.25 - main_loop.output;
      right = configs.PWM * 1.25 + main_loop.output;
    }

   // Setamos o PWM como 200 e os valores para calcular o output como 0
   // O PWM tá com o valor certo, mas o output tá dando "nan"
   // e provavelmente isso está zoando com a conta
   //Então o left e o right estão dando 0 como resultado final

  left = left - abs(main_loop.input)*configs.curveConstant;
  right = right - abs(main_loop.input)*configs.curveConstant;

  if (jaTentouFreiar) {
    if (mediaVelocidades < velocidadeParaAcabar[mappingPath - 1]) {
      jaFreiou = true;
    }
  }
  
  if (!mapeando&& !jaFreiou && ((CurrentVelocity - velocidadeParaAcabar[mappingPath-1])*50) > ((comprimentoTotal[mappingPath-1]-50) - ((contadorEncoderDireito + contadorEncoderEsquerdo)/2))
      && comprimentoTotal[mappingPath-1] < (contadorEncoderDireito + contadorEncoderEsquerdo)){
    jaTentouFreiar = true;
    left = -255 + main_loop.output;
    right = -255 - main_loop.output;
    main_loop.kp = configs.KpMain/8;
    //digitalWrite(LED_RIGHT, HIGH);
    }
  else{
    main_loop.kp = configs.KpMain;
    if (!mapeando){
      int d = 1;
      //digitalWrite(LED_RIGHT, LOW);
    }
    }

  setPWMs(left, right);
  
  /*cmdMessenger.sendCmdStart(1);
  cmdMessenger.sendCmdArg("Error: ");
  cmdMessenger.sendCmdArg(main_loop.error);
  cmdMessenger.sendCmdArg("\n");
  cmdMessenger.sendCmdEnd();*/
}

#endif
