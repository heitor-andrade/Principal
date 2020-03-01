#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include "CmdMessenger.h"
#include "control.h"
#include "motors.h"
#include <EEPROM.h>

CmdMessenger cmdMessenger(Serial2);
bool stop_requested = true;
bool calibrando = true;
bool terminou = false;

enum Commands
{
  Start = 1,
  Stop = 2,

  SetUseCascade = 3,
  SetTimeToStop = 4,
  SetStopByTime = 5,
  SetRightBlindTime = 6,
  SetStopByMarks = 7,

  Save = 8,
  Load = 9,

  SetKpMain = 10,
  SetKiMain = 11,
  SetKdMain = 12,
  SetDtMain = 13,
  SetSaturationMain = 14,
  SetPWM = 15,
  SetCurveConstant = 16,
  SetNextVelocity = 17,
  SetsecurityFactor = 18,
  SetTamanhoLinha = 19,

  SetKpLeft = 20,
  SetKiLeft = 21,
  SetKdLeft = 22,

  SetDtMotors = 23,
  SetSaturationMotors = 24,

  SetKpRight = 30,
  SetKiRight = 31,
  SetKdRight = 32,
};

void onUnknownCommand(CmdMessenger *messenger)
{
  cmdMessenger.sendCmdStart(-1);
  messenger->sendCmdArg("Comando desconhecido, lista de comandos:");
  cmdMessenger.sendCmdArg("\n");
  cmdMessenger.sendCmdArg(Commands::Start);
  cmdMessenger.sendCmdArg("Inicio do percurso");
  cmdMessenger.sendCmdArg("\n");
  cmdMessenger.sendCmdArg(Commands::Stop);
  cmdMessenger.sendCmdArg("Fim do percurso");
  cmdMessenger.sendCmdArg("\n");
  cmdMessenger.sendCmdArg(Commands::SetUseCascade);
  cmdMessenger.sendCmdArg("Modo Cascade:");
  cmdMessenger.sendCmdArg("\n");
  cmdMessenger.sendCmdArg(Commands::SetTimeToStop);
  cmdMessenger.sendCmdArg("Time to stop:");
  cmdMessenger.sendCmdArg("\n");
  cmdMessenger.sendCmdArg(Commands::SetStopByTime);
  cmdMessenger.sendCmdArg("Modo StopByTime:");
  cmdMessenger.sendCmdArg("\n");
  cmdMessenger.sendCmdArg(Commands::SetRightBlindTime);
  cmdMessenger.sendCmdArg("Right blind time");
  cmdMessenger.sendCmdArg("\n");
  cmdMessenger.sendCmdArg(Commands::SetStopByMarks);
  cmdMessenger.sendCmdArg("Modo StopByMarks");
  cmdMessenger.sendCmdArg("\n");
  cmdMessenger.sendCmdArg(Commands::Save);
  cmdMessenger.sendCmdArg("Configs salvas");
  cmdMessenger.sendCmdArg("\n");
  cmdMessenger.sendCmdArg(Commands::Load);
  cmdMessenger.sendCmdArg("Configs carregadas");
  cmdMessenger.sendCmdArg("\n");
  cmdMessenger.sendCmdArg(Commands::SetKpMain);
  cmdMessenger.sendCmdArg("KpMain");
  cmdMessenger.sendCmdArg("\n");
  cmdMessenger.sendCmdArg(Commands::SetKiMain);
  cmdMessenger.sendCmdArg("KiMain");
  cmdMessenger.sendCmdArg("\n");
  cmdMessenger.sendCmdArg(Commands::SetKdMain);
  cmdMessenger.sendCmdArg("KdMain");
  cmdMessenger.sendCmdArg("\n");
  cmdMessenger.sendCmdArg(Commands::SetDtMain);
  cmdMessenger.sendCmdArg("DtMain");
  cmdMessenger.sendCmdArg("\n");
  cmdMessenger.sendCmdArg(Commands::SetSaturationMain);
  cmdMessenger.sendCmdArg("SaturationMain");
  cmdMessenger.sendCmdArg("\n");
  cmdMessenger.sendCmdArg(Commands::SetPWM);
  cmdMessenger.sendCmdArg("PWM");
  cmdMessenger.sendCmdArg("\n");
  cmdMessenger.sendCmdArg(Commands::SetKpLeft);
  cmdMessenger.sendCmdArg("KpLeft");
  cmdMessenger.sendCmdArg("\n");
  cmdMessenger.sendCmdArg(Commands::SetKiLeft);
  cmdMessenger.sendCmdArg("KiLeft");
  cmdMessenger.sendCmdArg("\n");
  cmdMessenger.sendCmdArg(Commands::SetKdLeft);
  cmdMessenger.sendCmdArg("KdLeft");
  cmdMessenger.sendCmdArg("\n");
  cmdMessenger.sendCmdArg(Commands::SetDtMotors);
  cmdMessenger.sendCmdArg("DtMotors");
  cmdMessenger.sendCmdArg("\n");
  cmdMessenger.sendCmdArg(Commands::SetSaturationMotors);
  cmdMessenger.sendCmdArg("SaturationMotors");
  cmdMessenger.sendCmdArg("\n");
  cmdMessenger.sendCmdArg(Commands::SetKpRight);
  cmdMessenger.sendCmdArg("KpRight");
  cmdMessenger.sendCmdArg("\n");
  cmdMessenger.sendCmdArg(Commands::SetKiRight);
  cmdMessenger.sendCmdArg("KiRight");
  cmdMessenger.sendCmdArg("\n");
  cmdMessenger.sendCmdArg(Commands::SetKdRight);
  cmdMessenger.sendCmdArg("KdRight");
  cmdMessenger.sendCmdArg("\n");
  cmdMessenger.sendCmdEnd();
}

void onStartCommand(CmdMessenger *messenger) {
  cmdMessenger.sendCmdStart(Commands::Start);
  cmdMessenger.sendCmdArg("Inicio da calibração");
  cmdMessenger.sendCmdArg("\n");
  long unsigned int tempo = millis();
  while(millis() - tempo < 5000){
  calibrateSensors(); // sensors.h
  cmdMessenger.feedinSerialData();
  cmdMessenger.sendCmdArg("calibrando \n");
  }
  cmdMessenger.sendCmdArg("Fim da Calibração");
  cmdMessenger.sendCmdEnd();
  calibrando = false;
  stop_requested = false;
}

void onStopCommand(CmdMessenger *messenger) {
  cmdMessenger.sendCmdStart(Commands::Stop);
  cmdMessenger.sendCmdArg("Parada por bluetooth");
  cmdMessenger.sendCmdArg("\n");
  cmdMessenger.sendCmdEnd();
  setPWMs(FINAL_PWM, FINAL_PWM);
  stopRobot(); // motors.h
  resetControllers(); // control.h
  printAllValuesNewMode();
  stop_requested = true;
  terminou = true;
}

void onSetUseCascadeCommand(CmdMessenger *messenger) {
  configs.use_cascade = messenger->readBoolArg();
  cmdMessenger.sendCmdStart(Commands::SetUseCascade);
  cmdMessenger.sendCmdArg("Modo Cascade alterado:");
  cmdMessenger.sendCmdArg(configs.use_cascade);
  cmdMessenger.sendCmdArg("\n");
  cmdMessenger.sendCmdEnd();
}

void onSetTimeToStopCommand(CmdMessenger *messenger) {
  configs.time_to_stop = messenger->readInt32Arg();
  cmdMessenger.sendCmdStart(Commands::SetTimeToStop);
  cmdMessenger.sendCmdArg("Time to stop alterado:");
  cmdMessenger.sendCmdArg(configs.time_to_stop);
  cmdMessenger.sendCmdArg("\n");
  cmdMessenger.sendCmdEnd();

}

void onSetStopByTimeCommand(CmdMessenger *messenger) {
  configs.stop_by_time = messenger->readBoolArg();
  cmdMessenger.sendCmdStart(Commands::SetStopByTime);
  cmdMessenger.sendCmdArg("Modo StopByTime alterado:");
  cmdMessenger.sendCmdArg(configs.stop_by_time);
  cmdMessenger.sendCmdArg("\n");
  cmdMessenger.sendCmdEnd();
}

void onSetRightBlindTimeCommand(CmdMessenger *messenger) {
  configs.right_blind_time = messenger->readInt32Arg();
  cmdMessenger.sendCmdStart(Commands::SetRightBlindTime);
  cmdMessenger.sendCmdArg("Right blind time alterado");
  cmdMessenger.sendCmdArg(configs.right_blind_time);
  cmdMessenger.sendCmdArg("\n");
  cmdMessenger.sendCmdEnd();

}

void onSetStopByMarksCommand(CmdMessenger *messenger) {
  configs.stop_by_marks = messenger->readBoolArg();
  cmdMessenger.sendCmdStart(Commands::SetStopByMarks);
  cmdMessenger.sendCmdArg("Modo StopMyMarks alterado");
  cmdMessenger.sendCmdArg(configs.stop_by_marks);
  cmdMessenger.sendCmdArg("\n");
  cmdMessenger.sendCmdEnd();
}

void onSaveCommand(CmdMessenger *messenger) {
  EEPROM.put(0, configs);
  cmdMessenger.sendCmdStart(Commands::Save);
  cmdMessenger.sendCmdArg("Configs salvas");
  cmdMessenger.sendCmdArg("\n");
  cmdMessenger.sendCmdEnd();
  startPid();
}

void onLoadCommand(CmdMessenger *messenger) {
  EEPROM.get(0, configs);
  cmdMessenger.sendCmdStart(Commands::Load);
  cmdMessenger.sendCmdArg("Configs carregadas");
  cmdMessenger.sendCmdArg("\n");

  cmdMessenger.sendCmdArg(Commands::SetUseCascade);
  cmdMessenger.sendCmdArg("Modo Cascade:");
  cmdMessenger.sendCmdArg(configs.use_cascade);
  cmdMessenger.sendCmdArg("\n");

  cmdMessenger.sendCmdArg(Commands::SetTimeToStop);
  cmdMessenger.sendCmdArg("Time to stop:");
  cmdMessenger.sendCmdArg(configs.time_to_stop);
  cmdMessenger.sendCmdArg("\n");

  cmdMessenger.sendCmdArg(Commands::SetStopByTime);
  cmdMessenger.sendCmdArg("Modo StopByTime:");
  cmdMessenger.sendCmdArg(configs.stop_by_time);
  cmdMessenger.sendCmdArg("\n");

  cmdMessenger.sendCmdArg(Commands::SetRightBlindTime);
  cmdMessenger.sendCmdArg("Right blind time");
  cmdMessenger.sendCmdArg(configs.right_blind_time);
  cmdMessenger.sendCmdArg("\n");

  cmdMessenger.sendCmdArg(Commands::SetStopByMarks);
  cmdMessenger.sendCmdArg("Modo StopMyMarks");
  cmdMessenger.sendCmdArg(configs.stop_by_marks);
  cmdMessenger.sendCmdArg("\n");

  cmdMessenger.sendCmdArg(Commands::SetKpMain);
  cmdMessenger.sendCmdArg("KpMain");
  cmdMessenger.sendCmdArg(configs.KpMain);
  cmdMessenger.sendCmdArg("\n");

  cmdMessenger.sendCmdArg(Commands::SetKiMain);
  cmdMessenger.sendCmdArg("KiMain");
  cmdMessenger.sendCmdArg(configs.KiMain);
  cmdMessenger.sendCmdArg("\n");

  cmdMessenger.sendCmdArg(Commands::SetKdMain);
  cmdMessenger.sendCmdArg("KdMain");
  cmdMessenger.sendCmdArg(configs.KdMain);
  cmdMessenger.sendCmdArg("\n");

  cmdMessenger.sendCmdArg(Commands::SetDtMain);
  cmdMessenger.sendCmdArg("DtMain");
  cmdMessenger.sendCmdArg(configs.DtMain);
  cmdMessenger.sendCmdArg("\n");

  cmdMessenger.sendCmdArg(Commands::SetSaturationMain);
  cmdMessenger.sendCmdArg("SaturationMain");
  cmdMessenger.sendCmdArg(configs.SaturationMain);
  cmdMessenger.sendCmdArg("\n");

  cmdMessenger.sendCmdArg(Commands::SetPWM);
  cmdMessenger.sendCmdArg("PWM");
  cmdMessenger.sendCmdArg(configs.PWM);
  cmdMessenger.sendCmdArg("\n");

  cmdMessenger.sendCmdArg(Commands::SetKpLeft);
  cmdMessenger.sendCmdArg("KpLeft");
  cmdMessenger.sendCmdArg(configs.KpLeft);
  cmdMessenger.sendCmdArg("\n");

  cmdMessenger.sendCmdArg(Commands::SetKiLeft);
  cmdMessenger.sendCmdArg("KiLeft");
  cmdMessenger.sendCmdArg(configs.KiLeft);
  cmdMessenger.sendCmdArg("\n");

  cmdMessenger.sendCmdArg(Commands::SetKdLeft);
  cmdMessenger.sendCmdArg("KdLeft");
  cmdMessenger.sendCmdArg(configs.KdLeft);
  cmdMessenger.sendCmdArg("\n");

  cmdMessenger.sendCmdArg(Commands::SetDtMotors);
  cmdMessenger.sendCmdArg("DtMotors");
  cmdMessenger.sendCmdArg(configs.DtMotors);
  cmdMessenger.sendCmdArg("\n");

  cmdMessenger.sendCmdArg(Commands::SetSaturationMotors);
  cmdMessenger.sendCmdArg("SaturationMotors");
  cmdMessenger.sendCmdArg(configs.SaturationMotors);
  cmdMessenger.sendCmdArg("\n");

  cmdMessenger.sendCmdArg(Commands::SetKpRight);
  cmdMessenger.sendCmdArg("KpRight");
  cmdMessenger.sendCmdArg(configs.KpRight);
  cmdMessenger.sendCmdArg("\n");

  cmdMessenger.sendCmdStart(Commands::SetKiRight);
  cmdMessenger.sendCmdArg("KiRight");
  cmdMessenger.sendCmdArg(configs.KiRight);
  cmdMessenger.sendCmdArg("\n");

  cmdMessenger.sendCmdArg(Commands::SetKdRight);
  cmdMessenger.sendCmdArg("KdRight");
  cmdMessenger.sendCmdArg(configs.KdRight);
  cmdMessenger.sendCmdArg("\n");

  cmdMessenger.sendCmdArg(Commands::SetCurveConstant);
  cmdMessenger.sendCmdArg("CurveConstant");
  cmdMessenger.sendCmdArg(configs.curveConstant);
  cmdMessenger.sendCmdArg("\n");

  cmdMessenger.sendCmdEnd();
}

void onSetKpMainCommand(CmdMessenger *messenger) {
  configs.KpMain = messenger->readFloatArg();
  cmdMessenger.sendCmdStart(Commands::SetKpMain);
  cmdMessenger.sendCmdArg("KpMain alterado");
  cmdMessenger.sendCmdArg(configs.KpMain);
  cmdMessenger.sendCmdArg("\n");
  cmdMessenger.sendCmdEnd();
}

void onSetKiMainCommand(CmdMessenger *messenger) {
  configs.KiMain = messenger->readFloatArg();
  cmdMessenger.sendCmdStart(Commands::SetKiMain);
  cmdMessenger.sendCmdArg("KiMain alterado");
  cmdMessenger.sendCmdArg(configs.KiMain);
  cmdMessenger.sendCmdArg("\n");
  cmdMessenger.sendCmdEnd();
}

void onSetKdMainCommand(CmdMessenger *messenger) {
  configs.KdMain = messenger->readFloatArg();
  cmdMessenger.sendCmdStart(Commands::SetKdMain);
  cmdMessenger.sendCmdArg("KdMain alterado");
  cmdMessenger.sendCmdArg(configs.KdMain);
  cmdMessenger.sendCmdArg("\n");
  cmdMessenger.sendCmdEnd();
}

void onSetDtMainCommand(CmdMessenger *messenger) {
  configs.DtMain = messenger->readInt32Arg();
  cmdMessenger.sendCmdStart(Commands::SetDtMain);
  cmdMessenger.sendCmdArg("DtMain alterado");
  cmdMessenger.sendCmdArg(configs.DtMain);
  cmdMessenger.sendCmdArg("\n");
  cmdMessenger.sendCmdEnd();
}

void onSetSaturationMainCommand(CmdMessenger *messenger) {
  configs.SaturationMain = messenger->readInt32Arg();
  cmdMessenger.sendCmdStart(Commands::SetSaturationMain);
  cmdMessenger.sendCmdArg("SaturationMain alterado");
  cmdMessenger.sendCmdArg(configs.SaturationMain);
  cmdMessenger.sendCmdArg("\n");
  cmdMessenger.sendCmdEnd();
}

void onSetPWMCommand(CmdMessenger *messenger) {
  configs.PWM = messenger->readInt32Arg();
  cmdMessenger.sendCmdStart(Commands::SetPWM);
  cmdMessenger.sendCmdArg("PWM alterado");
  cmdMessenger.sendCmdArg(configs.PWM);
  cmdMessenger.sendCmdArg("\n");
  cmdMessenger.sendCmdEnd();
}

void onSetCurveConstantCommand(CmdMessenger *messenger) {
  configs.curveConstant = messenger->readFloatArg();
  cmdMessenger.sendCmdStart(Commands::SetCurveConstant);
  cmdMessenger.sendCmdArg("CurveConstant alterado");
  cmdMessenger.sendCmdArg(configs.curveConstant);
  cmdMessenger.sendCmdArg("\n");
  cmdMessenger.sendCmdEnd();
}

void onSetNextVelocityCommand(CmdMessenger *messenger) {
  configs.NextVelocity = messenger->readFloatArg();
  cmdMessenger.sendCmdStart(Commands::SetNextVelocity);
  cmdMessenger.sendCmdArg("NextVelocity alterado");
  cmdMessenger.sendCmdArg(configs.NextVelocity);
  cmdMessenger.sendCmdArg("\n");
  cmdMessenger.sendCmdEnd();
}

void onSetsecurityFactorCommand(CmdMessenger *messenger) {
  configs.securityFactor = messenger->readFloatArg();
  cmdMessenger.sendCmdStart(Commands::SetsecurityFactor);
  cmdMessenger.sendCmdArg("securityFactor alterado");
  cmdMessenger.sendCmdArg(configs.securityFactor);
  cmdMessenger.sendCmdArg("\n");
  cmdMessenger.sendCmdEnd();
}

void onSetTamanhoLinhaCommand(CmdMessenger *messenger) {
  configs.TamanhoLinha = messenger->readInt32Arg();
  cmdMessenger.sendCmdStart(Commands::SetTamanhoLinha);
  cmdMessenger.sendCmdArg("TamanhoLinha alterado");
  cmdMessenger.sendCmdArg(configs.TamanhoLinha);
  cmdMessenger.sendCmdArg("\n");
  cmdMessenger.sendCmdEnd();
}

void onSetKpLeftCommand(CmdMessenger *messenger) {
  configs.KpLeft = messenger->readFloatArg();
  cmdMessenger.sendCmdStart(Commands::SetKpLeft);
  cmdMessenger.sendCmdArg("KpLeft alterado");
  cmdMessenger.sendCmdArg(configs.KpLeft);
  cmdMessenger.sendCmdArg("\n");
  cmdMessenger.sendCmdEnd();
}

void onSetKiLeftCommand(CmdMessenger *messenger) {
  configs.KiLeft = messenger->readFloatArg();
  cmdMessenger.sendCmdStart(Commands::SetKiLeft);
  cmdMessenger.sendCmdArg("KiLeft alterado");
  cmdMessenger.sendCmdArg(configs.KiLeft);
  cmdMessenger.sendCmdArg("\n");
  cmdMessenger.sendCmdEnd();
}

void onSetKdLeftCommand(CmdMessenger *messenger) {
  configs.KdLeft = messenger->readFloatArg();
  cmdMessenger.sendCmdStart(Commands::SetKdLeft);
  cmdMessenger.sendCmdArg("KdLeft alterado");
  cmdMessenger.sendCmdArg(configs.KdLeft);
  cmdMessenger.sendCmdArg("\n");
  cmdMessenger.sendCmdEnd();
}

void onSetDtMotorsCommand(CmdMessenger *messenger) {
  configs.DtMotors = messenger->readInt32Arg();
  cmdMessenger.sendCmdStart(Commands::SetDtMotors);
  cmdMessenger.sendCmdArg("DtMotors alterado");
  cmdMessenger.sendCmdArg(configs.DtMotors);
  cmdMessenger.sendCmdArg("\n");
  cmdMessenger.sendCmdEnd();
}

void onSetSaturationMotorsCommand(CmdMessenger *messenger) {
  configs.SaturationMotors = messenger->readInt32Arg();
  cmdMessenger.sendCmdStart(Commands::SetSaturationMotors);
  cmdMessenger.sendCmdArg("SaturationMotors alterado");
  cmdMessenger.sendCmdArg(configs.SaturationMotors);
  cmdMessenger.sendCmdArg("\n");
  cmdMessenger.sendCmdEnd();
}

void onSetKpRightCommand(CmdMessenger *messenger) {
  configs.KpRight = messenger->readFloatArg();
  cmdMessenger.sendCmdStart(Commands::SetKpRight);
  cmdMessenger.sendCmdArg("KpRight alterado");
  cmdMessenger.sendCmdArg(configs.KpRight);
  cmdMessenger.sendCmdArg("\n");
  cmdMessenger.sendCmdEnd();
}

void onSetKiRightCommand(CmdMessenger *messenger) {
  configs.KiRight = messenger->readFloatArg();
  cmdMessenger.sendCmdStart(Commands::SetKiRight);
  cmdMessenger.sendCmdArg("KiRight alterado");
  cmdMessenger.sendCmdArg(configs.KiRight);
  cmdMessenger.sendCmdArg("\n");
  cmdMessenger.sendCmdEnd();
}

void onSetKdRightCommand(CmdMessenger *messenger) {
  configs.KiRight = messenger->readFloatArg();
  cmdMessenger.sendCmdStart(Commands::SetKdRight);
  cmdMessenger.sendCmdArg("KiRight alterado");
  cmdMessenger.sendCmdArg(configs.KiRight);
  cmdMessenger.sendCmdArg("\n");
  cmdMessenger.sendCmdEnd();
}

void attachCommands() {
  Serial2.begin(38400);
  cmdMessenger.attach(onUnknownCommand);
  cmdMessenger.attach(Commands::Start, onStartCommand);
  cmdMessenger.attach(Commands::Stop, onStopCommand);
  cmdMessenger.attach(Commands::SetUseCascade, onSetUseCascadeCommand);
  cmdMessenger.attach(Commands::SetTimeToStop, onSetTimeToStopCommand);
  cmdMessenger.attach(Commands::SetStopByTime, onSetStopByTimeCommand);
  cmdMessenger.attach(Commands::SetRightBlindTime, onSetRightBlindTimeCommand);
  cmdMessenger.attach(Commands::SetStopByMarks, onSetStopByMarksCommand);
  cmdMessenger.attach(Commands::Save, onSaveCommand);
  cmdMessenger.attach(Commands::Load, onLoadCommand);
  cmdMessenger.attach(Commands::SetKpMain, onSetKpMainCommand);
  cmdMessenger.attach(Commands::SetKiMain, onSetKiMainCommand);
  cmdMessenger.attach(Commands::SetKdMain, onSetKdMainCommand);
  cmdMessenger.attach(Commands::SetDtMain, onSetDtMainCommand);
  cmdMessenger.attach(Commands::SetSaturationMain, onSetSaturationMainCommand);
  cmdMessenger.attach(Commands::SetPWM, onSetPWMCommand);
  cmdMessenger.attach(Commands::SetCurveConstant, onSetCurveConstantCommand);
  cmdMessenger.attach(Commands::SetNextVelocity, onSetNextVelocityCommand);
  cmdMessenger.attach(Commands::SetsecurityFactor, onSetsecurityFactorCommand);
  cmdMessenger.attach(Commands::SetTamanhoLinha, onSetTamanhoLinhaCommand);
  cmdMessenger.attach(Commands::SetKpLeft, onSetKpLeftCommand);
  cmdMessenger.attach(Commands::SetKiLeft, onSetKiLeftCommand);
  cmdMessenger.attach(Commands::SetKdLeft, onSetKdLeftCommand);
  cmdMessenger.attach(Commands::SetDtMotors, onSetDtMotorsCommand);
  cmdMessenger.attach(Commands::SetSaturationMotors, onSetSaturationMotorsCommand);
  cmdMessenger.attach(Commands::SetKpRight, onSetKpRightCommand);
  cmdMessenger.attach(Commands::SetKiRight, onSetKiRightCommand);
  cmdMessenger.attach(Commands::SetKdRight, onSetKdRightCommand);
}

#endif
