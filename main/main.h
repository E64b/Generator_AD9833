#pragma once
#pragma pack(1, push)

#include <GyverTM1637.h>
#include <AD9833.h>
#include <Arduino.h>

#define FNC_PIN 4
#define CLK 2
#define DIO 3

extern uint16_t freq = 30000;
extern uint32_t periodns = 0;
extern volatile uint16_t cnt_ovf = 0;
extern inline __attribute__((always_inline))

extern GyverTM1637 disp;
extern AD9833 AD;

void SendSine();
void CalcFreq();
void Display();
void PeriodDelay();

#pragma pack(pop)