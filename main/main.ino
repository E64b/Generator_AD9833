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

void setup(){
    gen.Begin();
    display.setBrightness(0x0f); // Установка яркости дисплея   
    pinMode(3, OUTPUT);
}


/*Тут мы считаем паузу между сменой частоты*/
void PeriodDelay(){
    gen.EnableOutput(false);
    periodns = round(1000000000 / freq); //считаем период в наносекундах и округляем до целого числа
    while (Timer2 < (period / 2))
    {
    }
}

/*Тут мы выводим на экран текущую частоту*/
void Display(){
    display.showNumberDec(freq / 1000);
}

/*Высчитываем частоту с шагом 200Гц, если частота выше 999кГц возвращаемся на 30кГц*/
void CalcFreq(){
    for (freq; freq <= 999900;)
    {
        freq = freq + 200;
        if (freq > 999000)
        {
            freq = 30000;
        }
        break;
    }
}

/*Начинаем передачу сигнала*/
void SendSine(){
    gen.ApplySignal(SINE_WAVE, REG0, freq);
    gen.EnableOutput(true);
}

/*Выполняем по кругу все команды*/
void loop(){
  CalcFreq();
  Display();
  SendSine();
  PeriodDelay();
}
