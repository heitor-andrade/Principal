#ifndef MAPEAMENTO_H 
#define MAPEAMENTO_H

#include "encoder.h"
#include "bluetooth.h"

extern CmdMessenger cmdMessenger;

unsigned long lastTimeRead= 0;
int leftEncoderValues[100];
int rightEncoderValues[100];
unsigned long timeValues[100];
int mappingPath = 0;
bool sobreMarcacao = false;
bool jaPrintou = false;

bool jaTentouFreiar = false;
bool jaFreiou = false;

int contadorDeslocamentoTotal = 0;

String mappingData = "Resultados: \n";
String mappingDataCodeFormatLE = "[";
String mappingDataCodeFormatRE = "[";


void mudaMappingPath(){
  jaFreiou = false;
  jaTentouFreiar = false;
  leftEncoderValues[mappingPath] = contadorEncoderEsquerdo;
  rightEncoderValues[mappingPath] = contadorEncoderDireito;
  timeValues[mappingPath] = millis() - lastTimeRead;
  lastTimeRead = millis(); 
  
  cmdMessenger.sendCmdStart(1);
  cmdMessenger.sendCmdArg(mappingPath); 
  cmdMessenger.sendCmdArg("\n");
  cmdMessenger.sendCmdEnd();

  contadorDeslocamentoTotal += (contadorEncoderDireito + contadorEncoderEsquerdo)/2;
  contadorEncoderDireito = 0;
  contadorEncoderEsquerdo = 0;
  sobreMarcacao = true;
}

void readTics()
{
  if ((readLeft() || readRight()) && !sobreMarcacao && millis() - lastTimeRead > 100){ 
    mappingPath++;
    mudaMappingPath();
  }
  else
    sobreMarcacao = false;
}

void printAllValuesNewMode()
{
   if (!jaPrintou)
  {
    jaPrintou = true;
    mappingPath = 2;
    
    while (leftEncoderValues[mappingPath] != 0) 
    {   
      mappingPath++;
    }
   
    int posicao;
    for (posicao = 2;posicao < mappingPath-1; posicao++)
      mappingDataCodeFormatLE += String(leftEncoderValues[posicao]) + ",";
      
    mappingDataCodeFormatLE += String(leftEncoderValues[posicao]) + "]" + "\n";
    mappingData+= "Formato para Vetor(Lista) - Encoder esquerdo: " + mappingDataCodeFormatLE + "\n";

    for (posicao = 2;posicao < mappingPath-1; posicao++)
      mappingDataCodeFormatRE += String(rightEncoderValues[posicao]) + ",";
      
    mappingDataCodeFormatRE += String(rightEncoderValues[posicao]) + "]" + "\n";
    mappingData+= "Formato para Vetor(Lista) - Encoder direito: " + mappingDataCodeFormatRE + "\n";
  }
}

void fillZeros() {
  for (mappingPath = 0; mappingPath++; mappingPath <= 99) {
    leftEncoderValues[mappingPath] = 0;
    rightEncoderValues[mappingPath] = 0;
    timeValues[mappingPath] = 0;
  }
}

#endif
