#include "GyverTM1637.h"
#include <Arduino.h>
#include <avr/interrupt.h>
#include <avr/io.h>

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

volatile uint32_t freq = START_FREQ;
uint32_t step;
bool dispRedraw = true;
bool phase = false;

uint32_t PWM(float frequency) {
  TCCR1A = 1 << COM1A0;
  TCCR1C = 1 << FOC1A;
  if (frequency < (F_CPU / (65535UL * 16))) {
    TCCR1B = 1 << WGM12 | 1 << WGM13 | 0b011;
    ICR1 = ((float)F_CPU / (frequency * 128UL)) - 1;
    return ((float)F_CPU / ((ICR1 + 1) * 128.0f));
  } else if (frequency < (F_CPU / (65535UL * 2))) {
    TCCR1B = 1 << WGM12 | 1 << WGM13 | 0b010;
    ICR1 = ((float)F_CPU / (frequency * 16UL)) - 1;
    return ((float)F_CPU / ((ICR1 + 1) * 16.0f));
  } else {
    TCCR1B = 1 << WGM12 | 1 << WGM13 | 0b001;
    ICR1 = ((float)F_CPU / (frequency * 2UL)) - 1;
    return ((float)F_CPU / ((ICR1 + 1) * 2.0f));
  }
}

void Display() {
  if (dispRedraw) {
    disp.clear();
    disp.displayInt(freq / 1000);
    dispRedraw = false;
  }
}

void PhaseEdit() {
  if (phase) {
    Serial.println("Phase true");
    phase = false;
  } else {
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
    PWM(freq);
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
  PWM(freq);
}

void loop() {
  Display();
  FreqEdit();
}
