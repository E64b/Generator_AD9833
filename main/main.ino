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
    gen.ApplySignal(SQUARE_WAVE, REG1, freq);
    gen.ApplySignal(SQUARE_WAVE, REG0, freq);
    gen.IncrementPhase(REG0, 0);
    gen.IncrementPhase(REG1, 180);
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
        Disp = true;
    }
    else
    {
        freq = 30000;
        Disp = true;
    }
      
    /*Если частота обновилась выводим на дисплей*/
    if (Disp)
    {
        disp.clear();
        disp.displayInt(freq / 1000);
        Disp = false;
    }
    
    if (Phase == true)
    {
           //gen.EnableOutput(false);
           gen.SetFrequency(REG0, freq);
           gen.SetOutputSource(REG0);
           Phase = false;
    }
    else
    {
            //gen.EnableOutput(false);
            gen.SetFrequency(REG1, freq);
            gen.SetOutputSource(REG1);
            Phase = true;
    }
    gen.EnableOutput(true);
    delay(87);
}