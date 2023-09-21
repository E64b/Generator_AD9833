#include <GyverTM1637.h>
#include "AD9833.h"

#define FNC_PIN 10
#define CLK 2
#define DIO 3

uint32_t freq;
uint32_t periodns;

float Timer;
bool Disp = false;
bool Phase = true;
GyverTM1637 disp(CLK, DIO);
AD9833 gen(FNC_PIN);



void setup()
{   
    /*Инициализация AD9833*/
    gen.Begin();
    gen.ApplySignal(HALF_SQUARE_WAVE, REG0, freq);
    gen.EnableOutput(true);

    /*Инициализация дисплея*/
    disp.clear();
    disp.brightness(7);
    disp.point(0);        
}

/*Выполняем все по кругу*/
void loop(){
    /*Меняем частоту*/

    if ((freq <= 999000) and (freq >= 30000))
    {
        freq = freq + 200;
        disp = true;
    }
    else
    {
        freq = 30000;
        disp = true;
    }


    /*Если частота обновилась выводим на дисплей*/

    if (Disp)
    {
        disp.clear();
        disp.displayInt(freq / 1000);
        disp = false;
    }


    /*Считаем период текущей частоты*/

    periodns = round(1000000000.0f / freq);//Считаем период в наносекундах
    periodns = periodns / 2; //считаем полупериод


    /*Отправляем частоту */

    gen.ApplySignal(HALF_SQUARE_WAVE, REG0, freq);
    gen.EnableOutput(true);
    if (Phase == true)
    {
        gen.IncrementPhase(REG0, 0); //Ставим фазу по умолчанию
        Phase = false;
    }
    else
    {
        gen.IncrementPhase(REG0, 180); //Смещаем фазу на 180
        Phase = true;
    }

    delay(100);
}