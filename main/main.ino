#include <GyverTM1637.h>
#include <AD9833.h>
#include <Arduino.h>
//#define FNC_PIN 4
#define CLK 2
#define DIO 3

uint16_t freq = 30000;
uint32_t periodns = 0;
volatile uint16_t cnt_ovf = 0;
inline __attribute__((always_inline))

GyverTM1637 disp(CLK, DIO);
AD9833 AD;

void setup() {
  Serial.begin(9600);
  AD.begin(10);
  AD.setWave(AD9833_SINE);
  disp.clear();
  disp.brightness(7);
  disp.point(0);
  TCCR1A = TCCR1B = TCNT1 = cnt_ovf = 0;
  TIFR1 |= (1 << TOV1);
  TIMSK1 = (1 << TOIE0);
  TCCR1B = (1 << CS10);
}

ISR(TIMER1_OVF_vect) {
  cnt_ovf++;
}

/*Считаем период текущей частоты и устанавливаем паузу перед следующей*/
void PeriodDelay() {
  uint32_t Timer = 0;
  AD.setWave(AD9833_OFF);
  periodns = round(1000000000 / freq);
  Serial.println(periodns) ; //Считаем период в наносекундах
  while (Timer < (periodns / 2)) {
    TCCR1B = 0;                             // остановить таймер
    uint32_t count = TCNT1 - 2;             // минус два такта на действия
    count += ((uint32_t)cnt_ovf * 0xFFFF);  // с учетом переполнений
    Timer = Timer + (count * (float)(1000000000.0f / F_CPU), 0);
    Serial.println(Timer);
  }
}

/*Выводим текущую частоту на дисплей*/
void Display() {
  disp.clear();
  disp.displayInt(freq / 1000);
}

/*Отправляем сигнал*/
void SendSine() {
  AD.setFrequency(freq, 0);
  AD.setWave(AD9833_SINE);
}

/*Перебираем все частоты с шагом 200Гц, при достижении 999кГц возвращаемся обратно на 30кГц*/
void CalcFreq() {
  for (freq; freq <= 999900;) {
    freq = freq + 200;
    if (freq > 999000) {
      freq = 30000;
    }
    break;
  }
}

/*Выполняем все по кругу*/
void loop() {
  CalcFreq();
  Display();
  SendSine();
  PeriodDelay();
}
