#include <GyverTM1637.h>
#include "AD9833.h"

#define FNC_PIN 10
#define CLK 2
#define DIO 3

uint32_t freq = 30000;
uint32_t periodns;

float Timer;
bool Disp = true;
bool Phase = true;
GyverTM1637 disp(CLK, DIO);
AD9833 gen(FNC_PIN);



void setup()
{   
    /*Инициализация AD9833*/
    gen.Begin();
    gen.ApplySignal(HALF_SQUARE_WAVE, REG1, freq);
    gen.EnableOutput(true);

    /*Инициализация дисплея*/
    disp.clear();
    disp.brightness(7);
    disp.point(0);        
}

/*Выполняем все по кругу*/
void loop(){
    /*Меняем частоту*/
    /*
    if ((freq <= 999000) and (freq >= 30000))
    {
        freq = freq + 200;
        Disp = true;
    }
    else
    {
        freq = 30000;
        Disp = true;
    }

    */
    /*Если частота обновилась выводим на дисплей*/
    Disp = true; //временно
    if (Disp)
    {
        disp.clear();
        disp.displayInt(freq / 1000);
        Disp = false;
    }

    /*Отправляем частоту */      
    if (Phase == true)
    {
        gen.IncrementPhase(REG1, 0); //Ставим фазу по умолчанию
        Phase = false;
    }
    else
    {
        gen.IncrementPhase(REG1, 180); //Смещаем фазу на 180
        Phase = true;
    }
    gen.SetFrequency(REG1, freq);
    gen.SetOutputSource(REG1);
    gen.EnableOutput(true);
}