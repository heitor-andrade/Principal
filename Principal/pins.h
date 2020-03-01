#ifndef PINS_H
#define PINS_H

#define LED_LEFT 25
#define LED_RIGHT 4

#define LED_PIN 23
#define LED_PIN2 24

#define SWITCH_A 6
#define SWITCH_B 7
#define SWITCH_C 8
#define SWITCH_D 9

#define BUZZER 28

#define BATTERY 69

#define L_MOTOR_1 10
#define L_MOTOR_2 11
#define R_MOTOR_1 13
#define R_MOTOR_2 12

#define BUTTON_PIN 5

#define ENCODER_RIGHT_1 20
#define ENCODER_RIGHT_2 21
#define ENCODER_LEFT_1 18
#define ENCODER_LEFT_2 19



void setupPins() {
  pinMode(R_MOTOR_1, OUTPUT);
  pinMode(R_MOTOR_2, OUTPUT);
  pinMode(L_MOTOR_1, OUTPUT);
  pinMode(L_MOTOR_2, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(LED_PIN2, OUTPUT);
  pinMode(LED_LEFT, OUTPUT);
  pinMode(LED_RIGHT, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);
  pinMode(SWITCH_A, INPUT);
  pinMode(SWITCH_B, INPUT);
  pinMode(SWITCH_C, INPUT);
  pinMode(SWITCH_D, INPUT);
  pinMode(BATTERY, INPUT);
}

#endif
