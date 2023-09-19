#include <SPI.h>
#include <TM1637Display.h> // Подключение библиотеки для управления дисплеем TM1637 
#include <AD9833.h>
#include <Arduino.h>

#define FNC_PIN 4
#define CLK 2 // Определение CLK для подключения дисплея TM1637 
#define DIO 3 // Определение DIO для подключения дисплея TM1637 

AD9833 gen(FNC_PIN);
TM1637Display display(CLK, DIO);

uint16_t freq = 30000;
uint32_t period = 0;
volatile uint16_t cnt_ovf = 0;
inline __attribute__((always_inline))

void test(void){
    asm("nop");
}


void setup(){
    gen.Begin();
    display.setBrightness(0x0f); // Установка яркости дисплея  
     TCCR1A = TCCR1B = TCNT1 = cnt_ovf = 0;  // Сброс таймера
  TIFR1 |= (1 << TOV1);
  TIMSK1 = (1 << TOIE0);                  // Прерывание переполнения
  TCCR1B = (1 << CS10);                   // Старт таймера
  test();                                 // тест
  TCCR1B = 0;                             // остановить таймер
  uint32_t count = TCNT1 - 2;             // минус два такта на действия
  count += ((uint32_t)cnt_ovf * 0xFFFF);  // с учетом переполнений
  
}

ISR(TIMER1_OVF_vect)
{
    cnt_ovf++;
}
/*Выполняем по кругу все команды*/
void loop(){
  CalcFreq();
  Display();
  SendSine();
  PeriodDelay();
}
