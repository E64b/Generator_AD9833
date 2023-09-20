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

float Timer;

void setup() {
  AD.begin(10); //Пин для AD9833
  AD.setWave(AD9833_SINE);
  disp.clear();
  disp.brightness(7);
  disp.point(0);
  /*Первичная инициализация таймера*/
  TCCR1A = TCCR1B = TCNT1 = cnt_ovf = 0;
  TIFR1 |= (1 << TOV1);
  TIMSK1 = (1 << TOIE0);
  TCCR1B = (1 << CS10);
}

ISR(TIMER1_OVF_vect) {
  cnt_ovf++;
}

/*Считаем период текущей частоты и устанавливаем паузу перед следующей*/
void PeriodDelay(){
  float Timer = 0; //Обнуляем переменную
  AD.setWave(AD9833_OFF); //Выключаем передачу сигнала
  periodns = round(1000000000 / freq);//Считаем период в наносекундах
  TCCR1A = TCCR1B = TCNT1 = cnt_ovf = 0; //Сбрасываем таймер
  TCCR1B = (1 << CS10); //Запускаем таймер
  /*Сидим в этом цикле пока не пройдет полупериод*/
   while (Timer <= (periodns / 2)){    
    uint32_t count = TCNT1 - 2; // минус два такта на действия
    count += ((uint32_t)cnt_ovf * 0xFFFF);  // с учетом переполнений
    Timer = Timer + (count * (1000000000.0f / F_CPU)); //Считаем время
  }
  TCCR1B = 0; // остановить таймер
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
