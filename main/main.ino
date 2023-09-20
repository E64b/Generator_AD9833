#include <GyverTM1637.h>
#include "AD9833.h"

#define FNC_PIN 10
#define CLK 2
#define DIO 3

uint64_t freq;
uint64_t periodns;
volatile uint16_t cnt_ovf = 0;
inline __attribute__((always_inline))
float Timer;
uint64_t peri = 0;
GyverTM1637 disp(CLK, DIO);
AD9833 gen(FNC_PIN);

void setup() 
{
  gen.Begin();
  gen.ApplySignal(SINE_WAVE, REG0, freq);
  gen.EnableOutput(true);
  disp.clear();
  disp.brightness(7);
  disp.point(0);
  /*Первичная инициализация таймера*/
  TCCR1A = TCCR1B = TCNT1 = cnt_ovf = 0;
  TIFR1 |= (1 << TOV1);
  TIMSK1 = (1 << TOIE0);
  TCCR1B = (1 << CS10);
  freq = 30000;
}

ISR(TIMER1_OVF_vect) 
{
  cnt_ovf++;
}


/*Считаем период текущей частоты и устанавливаем паузу перед следующей*/
void PeriodDelay()
{
  peri = 0;
  Timer = 0; //Обнуляем переменную   
  periodns = round(1000000000 / freq);//Считаем период в наносекундах
  TCCR1A = TCCR1B = TCNT1 = cnt_ovf = 0; //Сбрасываем таймер
  TCCR1B = (1 << CS10); //Запускаем таймер
  peri = periodns / 2;
  /*Сидим в этом цикле пока не пройдет полупериод*/
   while (Timer <= (peri))
   { 
    gen.EnableOutput(false);  //Выключаем передачу сигнала 
    uint32_t count = TCNT1 - 2; // минус два такта на действия
    count += ((uint32_t)cnt_ovf * 0xFFFF);  // с учетом переполнений
    Timer = Timer + (count * (1000000000.0f / F_CPU)); //Считаем время
  }
  TCCR1B = 0; // остановить таймер
}


/*Выполняем все по кругу*/
void loop() 
{
  /*Считаем частоту*/
  if ((freq <= 999000) and (freq >= 30000))
    {
      freq = freq + 200;
    }
    else 
    {
      freq = 30000;
    }
/*Отправляем частоту и выводим на дисплей*/
    gen.ApplySignal(SQUARE_WAVE, REG0, freq);
    gen.EnableOutput(true); 
    disp.clear();
    disp.displayInt(freq / 1000);
    PeriodDelay();
}
