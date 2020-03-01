#ifndef CONTROL_H
#define CONTROL_H

#include "constants.h"
#include "CmdMessenger.h"

int contadorEncoderDireito = 0;
int contadorEncoderEsquerdo = 0;
float valor_absurdo = 3.00;
bool sentido;
int filtro = 0, contador = 0;


struct Configs
{
  bool use_cascade, stop_by_time, stop_by_marks;
  unsigned int PWM;
  unsigned long time_to_stop, right_blind_time;
  float linear,KpMain,KdMain,KiMain,KpLeft,KdLeft,KiLeft,KpRight,KdRight,KiRight, curveConstant, NextVelocity, securityFactor;
  int SaturationMain, SaturationMotors, DtMain, DtMotors, TamanhoLinha; 
};

Configs configs;

struct ControlLoop {
  float kp, ki, kd, error, last_error = 0, pre_error = 0, integral = 0, derivative = 0, output = 0, reference = 0, saturation, input = 0;
  int dt, encoderCounter = 0;
  unsigned long last_run = 0;
};

ControlLoop main_loop;
ControlLoop left_motor_loop;
ControlLoop right_motor_loop;

void startPid(){
  main_loop.dt = configs.DtMain;
  main_loop.saturation = configs.SaturationMain;
  main_loop.kp = configs.KpMain;
  main_loop.ki = configs.KiMain;
  main_loop.kd = configs.KdMain;
  main_loop.last_run = 0;

  left_motor_loop.dt = configs.DtMotors;
  left_motor_loop.saturation = configs.SaturationMotors;
  left_motor_loop.kp = configs.KpLeft;
  left_motor_loop.ki = configs.KiLeft;
  left_motor_loop.kd = configs.KdLeft;    
  contadorEncoderEsquerdo = 0;

  right_motor_loop.dt = configs.DtMotors;
  right_motor_loop.saturation = configs.SaturationMotors;
  right_motor_loop.kp = configs.KpRight;
  right_motor_loop.ki = configs.KiRight;
  right_motor_loop.kd = configs.KdRight; 
  contadorEncoderDireito = 0; 
}

void resetControllers() {
  main_loop.last_error = 0;
  main_loop.pre_error = 0;
  main_loop.integral = 0;
  main_loop.last_run = millis();

  left_motor_loop.last_error = 0;
  left_motor_loop.integral = 0;
  left_motor_loop.last_run = millis();

  right_motor_loop.last_error = 0;
  right_motor_loop.integral = 0;
  right_motor_loop.last_run = millis();
}

void calculateControlSignal(struct ControlLoop * this_loop) { // verify in the future the need of a filter for the derivative
  if (millis() - this_loop->last_run >= this_loop->dt) { // udpate output only if dt passed
    
    this_loop->error = this_loop->reference - this_loop->input;
   

    // a variável sentido acompanha as curvas
    if(this_loop->error > 0 && this_loop->error <2.5){
      sentido = 1;
    }

    if(this_loop->error < 0 && this_loop->error > -2.5){
      sentido = 0;
    }
    
    if (abs(this_loop->error) >= valor_absurdo) {
    filtro += 1;
    contador += 1;  
    }
    
    if (contador > 20 && filtro > 10){
      if(sentido) 
        this_loop->error = 4;
      else
        this_loop->error = -4;
    }

    if (abs(this_loop->error < 0.6)){
       filtro = 0;
       contador =0;
    }

    
    this_loop->integral += this_loop->ki * this_loop->error * this_loop->dt / 1000.0; // dividing by 1000 because millis    
    this_loop->derivative = 1000 * (this_loop->error - this_loop->last_error) / this_loop->dt; // multiplying by 1000 because millis
    
    this_loop->output = this_loop->kp * this_loop->error + this_loop->integral + this_loop->kd * this_loop->derivative; // pid
    
    this_loop->pre_error = this_loop->last_error;
    this_loop->last_error = this_loop->error; 
    this_loop->last_run = millis();
  }
}

void updateCascade() {
  left_motor_loop.reference = configs.linear - main_loop.output * DISTANCIA_ENTRE_RODAS * 0.5;
  right_motor_loop.reference = configs.linear + main_loop.output * DISTANCIA_ENTRE_RODAS * 0.5;
}

#endif
