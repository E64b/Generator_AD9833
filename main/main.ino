#include "GyverTM1637.h"
#include "GyverTimers.h"
#include <Arduino.h>
#include <inttypes.h>
#include <SPI.h>

/*Pins for display*/
#define CLK 2
#define DIO 3

//
// PWM OUT D13
//

/*Setup FREQ*/
#define START_FREQ 30000
#define MAX_FREQ 998800
#define STEP_FREQ 200
#define TIME_STEP 87

GyverTM1637 disp(CLK, DIO);

uint32_t Freq = START_FREQ;
uint32_t Step = 0;
bool dispRedraw = true;

void Display() {
  if (dispRedraw) {
    disp.clear();
    disp.displayInt(Freq / 1000);
    dispRedraw = false;
  }
}

void FreqEdit() {
  /*Меняем частоту*/
  if (Freq <= MAX_FREQ && Freq >= START_FREQ) {
    Freq = Freq + STEP_FREQ;
  } else {
    Freq = START_FREQ;
  }
}

void setup() {
  //Timer1.setFrequency(START_FREQ);
  Timer1.enableISR(CHANNEL_A);
  disp.clear();
  disp.brightness(7);
  disp.point(0);
  pinMode(9, OUTPUT);
}

void loop() {
  Display();
  if (millis() - Step >= TIME_STEP) {
    Step = millis();
    dispRedraw = true;
    FreqEdit();
    Timer1.setFrequency(Freq);
    Timer1.restart();
    
  }
}

ISR(TIMER1_A) { digitalWrite(9, !digitalRead(9)); }