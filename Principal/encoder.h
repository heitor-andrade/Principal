// VELOCIDADES GIRAM EM TORNO DE 0-1 m/s

#ifndef ENCODERS_H
#define ENCODERS_H

#include "Pins.h"
//#include "control.h"


#define RAIO_RODA 10
#define TAM_FILTRO_VELOCIDADE 15

//attach interrupts on rising ENCODER_RIGHT_1 ENCODER_LEFT_1

volatile long t_direito = micros();
volatile long dt_direito = 1000000;
float velocidadeRight;
volatile long t_esquerdo = micros();
volatile long dt_esquerdo = 1000000;
float velocidadeLeft;
float CurrentVelocity;
float mediaVelocidades = 0;
float listaVelocidades[TAM_FILTRO_VELOCIDADE] = {0};
long contadorListaVelocidades = 0;

void readEncoderRightRising() {
  
  unsigned volatile long registro_direita = micros();
  dt_direito = registro_direita - t_direito;
  t_direito = registro_direita;
      
  if (digitalRead(ENCODER_RIGHT_2)){
    contadorEncoderDireito--;
    dt_direito = -dt_direito;
  }
  else{
    contadorEncoderDireito++; 
  }
    
}

void readEncoderLeftRising() {
  unsigned volatile long registro_esquerdo = micros();
  dt_esquerdo = registro_esquerdo - t_esquerdo;
  t_esquerdo = registro_esquerdo;
  if (digitalRead(ENCODER_LEFT_2)){
    contadorEncoderEsquerdo++;
  }
  else{
    contadorEncoderEsquerdo--; 
    dt_esquerdo = -dt_esquerdo;
  } 
}

/* Tem o objetivo de tentar dobrar a resolucao do encoder futuramente
void readEncoderRightFalling() {
  unsigned volatile long registro_direita = micros();
  dt_direito = registro_direita - t_direito;
  t_direito = registro_direita;
      
  if (digitalRead(ENCODER_RIGHT_2)){
    contadorEncoderDireito++;
  }
  else{
    contadorEncoderDireito--;  
    dt_direito = -dt_direito;
  }
    
}

void readEncoderLeftFalling() {
  unsigned volatile long registro_esquerdo = micros();
  dt_esquerdo = registro_esquerdo - t_esquerdo;
  t_esquerdo = registro_esquerdo;
  
  if (digitalRead(ENCODER_LEFT_2)){
    contadorEncoderEsquerdo--; 
  }
  else{
    contadorEncoderEsquerdo++; 
    dt_esquerdo = -dt_esquerdo;
  }  
}
*/

void startInts() {
  attachInterrupt(digitalPinToInterrupt(ENCODER_RIGHT_1), readEncoderRightRising, RISING);
  attachInterrupt(digitalPinToInterrupt(ENCODER_LEFT_1), readEncoderLeftRising, RISING);
  pinMode(ENCODER_RIGHT_2, INPUT);
  pinMode(ENCODER_LEFT_2, INPUT);
}

void calculateSpeeds(){
  
  velocidadeLeft = 2000 * PI * RAIO_RODA/(dt_esquerdo*RESOLUCAO_ENCODER);
  velocidadeRight = 2000 * PI * RAIO_RODA/(dt_direito*RESOLUCAO_ENCODER);
  CurrentVelocity = min(velocidadeLeft,velocidadeRight);
  mediaVelocidades = 0;
  for(int i = 0; i < TAM_FILTRO_VELOCIDADE; i++)
  {
    mediaVelocidades += listaVelocidades[i];
  }
  mediaVelocidades /= TAM_FILTRO_VELOCIDADE;
  listaVelocidades[contadorListaVelocidades%TAM_FILTRO_VELOCIDADE] = CurrentVelocity;
  contadorListaVelocidades ++;
}

#endif
