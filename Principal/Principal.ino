#include <EEPROM.h>
#include "pins.h"
#include "sensors.h"
#include "control.h"
#include "bluetooth.h"
#include "motors.h"
#include "constants.h"
#include "encoder.h"
#include "mapeamento.h"
#include "track.h"

bool on_the_run = false;
unsigned long run_start;
bool botaoParar = false;
int state = 0;
float c = 330;

void setup() {
  EEPROM.get(0, configs);
  setupPins(); // pins.h
  attachCommands(); // bluetooth.h
  startInts(); //encoder.h
  fillZeros();//mapeamento.h
  stopRobot();
  resetControllers();

  digitalWrite(LED_PIN, HIGH);
  Serial.begin(9600);

  configs.use_cascade = false; // bluetooth.h ; forcing to never use cascade (should be removed after tests)

  cmdMessenger.sendCmdStart(1);
  cmdMessenger.sendCmdArg(" Começou! ");
  cmdMessenger.sendCmdArg("\n");
  cmdMessenger.sendCmdEnd();
  while (calibrando) {
    if (digitalRead(SWITCH_A)) {
      cmdMessenger.feedinSerialData();
      cmdMessenger.sendCmdStart(1);
      cmdMessenger.sendCmdArg("LOOP 1");
      cmdMessenger.sendCmdArg("\n");
      cmdMessenger.sendCmdEnd();

    }
    else {
      while (not(buttonPressed())) {
        cmdMessenger.feedinSerialData();
      }
      delay(500);
      while (stop_requested) { // bluetooth.h
        cmdMessenger.feedinSerialData();
        calibrateSensors(); // sensors.h
        if (buttonPressed()) { // sensors.h
          stop_requested = false; // bluetooth.h
          calibrando = false;
        }
      }
    }
  }
  cmdMessenger.sendCmdStart(1);
  cmdMessenger.sendCmdArg("Calibrado");
  cmdMessenger.sendCmdArg("\n");
  cmdMessenger.sendCmdEnd();

  startPid();//control.h
  delay(500);
}

void loop() {

  cmdMessenger.feedinSerialData();

  digitalWrite(LED_PIN, HIGH);
  calculateSpeeds();
  readTics();

  updateIRs(); // sensors.h

  if (stop_requested == false) {
    calculateControlSignal(&main_loop); // motors.h (function) and control.h (parameter)
    moveSimple(); // motors.h
  }

  if (terminou == true) {
    terminou = false;
    delay(5000);
    cmdMessenger.sendCmdStart(0);
    cmdMessenger.sendCmdArg(mappingData);
    cmdMessenger.sendCmdArg("\n");
    cmdMessenger.sendCmdEnd();

  }

  if (configs.use_cascade = false) {
    calculateControlSignal(&main_loop); // control.h
    updateCascade(); // control.h
    calculateControlSignal(&left_motor_loop); // control.h
    calculateControlSignal(&right_motor_loop); // control.h
    //moveCascade(); // motors.h
  }
  else {


  }





  /* if (mappingPath == 41){

    digitalWrite(LED_LEFT, HIGH);
    digitalWrite(LED_RIGHT, HIGH);
    on_the_run = false;
            stop_requested = true;
            setPWMs(-255, -255);
            long tempinho = millis();
            while (((millis() - tempinho) < 150 + switchCLigado) || CurrentVelocity > 0.2){
              calculateSpeeds();
            }
            setPWMs(0, 0);
            delay(FINAL_DELAY);
    }*/


  if (buttonPressed()) { // sensors.h
    botaoParar = true; // bluetooth.h
  }


  /*if (detectMark(1)) { // sensors.h ; 1 because we're reading the rightIR
    if (not(on_the_run)) {
     on_the_run = true;
     run_start = millis();
    }
    else {
     if ((configs.stop_by_marks) and (millis() - run_start > configs.right_blind_time)) { // bluetooth.h
       on_the_run = false;
       stop_requested = true;
       setPWMs(FINAL_PWM, FINAL_PWM);
       delay(FINAL_DELAY);
     }
    }
    }

    if ((configs.stop_by_time) and (on_the_run)) { // bluetooth.h
    if (millis() - run_start > configs.time_to_stop) { // bluetooth.h
     on_the_run = false;
     stop_requested = true;
     setPWMs(FINAL_PWM, FINAL_PWM);
     delay(FINAL_DELAY);
    }
    }
    if (contadorDeslocamentoTotal > 18300 + switchDLigado || ((mappingPath == 38 && (contadorEncoderDireito + contadorEncoderEsquerdo) > 1100 ) && on_the_run)) { //Criterios de parada VERIFICACAO DE PARADA COM DESLOCAMENTO
           on_the_run = false;
           stop_requested = true;
           setPWMs(-255, -255);
           long tempinho = millis();
           while (((millis() - tempinho) < 300 + switchCLigado) || CurrentVelocity > 0.2){
             calculateSpeeds();
           }
           setPWMs(0, 0);
           delay(FINAL_DELAY);

    }*/

  while (botaoParar = false) { // bluetooth.h
    stopRobot(); // motors.h
    resetControllers(); // control.h
    printAllValuesNewMode();

    cmdMessenger.sendCmdStart(1);
    cmdMessenger.sendCmdArg("botão apertado! ");
    cmdMessenger.sendCmdArg("\n");
    cmdMessenger.sendCmdEnd();

    if (Serial2.available() > 0)
      state = Serial2.read();

    if (state == '1')
    {
      cmdMessenger.sendCmdStart(0);
      cmdMessenger.sendCmdArg(mappingData);
      cmdMessenger.sendCmdArg("\n");
      cmdMessenger.sendCmdEnd();
      state = 0;
    }
  }
}
