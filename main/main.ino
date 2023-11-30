#include "GyverTM1637.h"
#include "GyverTimers.h"
#include <Arduino.h>
#include <SPI.h>
#include <inttypes.h>

/*Pins for display*/
#define CLK 2
#define DIO 3

/*Setup FREQ*/
#define START_FREQ 30000
#define MAX_FREQ 998800
#define STEP_FREQ 200
#define TIME_STEP 87
#define PWM_PIN 9

GyverTM1637 disp(CLK, DIO);

uint32_t freq = START_FREQ;
uint32_t step = 0;
bool dispRedraw = true;
bool phase = false;

void Display() {
  if (dispRedraw) {
    disp.clear();
    disp.displayInt(freq / 1000);
    dispRedraw = false;
  }
}

void PhaseEdit() {
  if (phase) {
    Timer1.phaseShift(CHANNEL_A, 0);
    Serial.println("True");
    phase = false;
  } else {
    Timer1.phaseShift(CHANNEL_A, 180);
    Serial.println("False");
    phase = true;
  }
}

void FreqEdit() {
  /*Меняем частоту*/
  if (millis() - step >= TIME_STEP) {
    step = millis();
    dispRedraw = true;
    if (freq <= MAX_FREQ && freq >= START_FREQ) {
      freq = freq + STEP_FREQ;
    } else {
      freq = START_FREQ;
    }
    PhaseEdit();
    Serial.print("freq: ");
    Serial.print(freq);
    Serial.println("hz");
    Timer1.setFrequency(freq*2);
  }
}

void setup() {
  Serial.begin(115200);
  while (!Serial) {
  }
  Serial.println("Serial OK!");
  Timer1.setFrequency(freq*2);
  disp.clear();
  disp.brightness(7);
  disp.point(0);
  pinMode(PWM_PIN, OUTPUT);
  step = millis();
  Timer1.outputEnable(CHANNEL_A, PWM_PIN);
}

void loop() {
  Display();
  FreqEdit();
}
