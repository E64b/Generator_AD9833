#include "GyverPWM.h"
#include "GyverTM1637.h"
#include "GyverTimers.h"
#include <Arduino.h>
#include <avr/interrupt.h>
#include <avr/io.h>

/*Pins for display*/
#define CLK 2
#define DIO 3

/*Setup FREQ*/
#define START_FREQ 0
#define MAX_FREQ 100000
#define STEP_FREQ 200
#define TIME_STEP 100
#define PWM_PIN 9

GyverTM1637 disp(CLK, DIO);

volatile uint32_t freq = START_FREQ;
uint32_t step;
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
    //Timer1.phaseShift(CHANNEL_A, 0);
    Serial.println("Phase true");
    phase = false;
  } else {
    //Timer1.phaseShift(CHANNEL_A, 360);
    Serial.println("Phase false");
    phase = true;
  }
}

void FreqEdit() {

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
    Timer1.setFrequencyFloat(freq * 2);
    //Timer1.restart();
  }
}

void setup() {
  Serial.begin(115200);
  while (!Serial) {
  }
  Serial.println("Serial OK!");
  pinMode(PWM_PIN, OUTPUT);
  disp.clear();
  disp.brightness(7);
  disp.point(0);
  step = millis();
  Timer1.setFrequencyFloat(freq * 2);
  Timer1.outputEnable(CHANNEL_A, PWM_PIN);
}

void loop() {
  Display();
  FreqEdit();
}
