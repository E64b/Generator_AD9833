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
    Serial.begin(9600);
    /*Инициализация AD9833*/
    gen.Begin();
    gen.ApplySignal(SQUARE_WAVE, REG0, freq);
    gen.SetPhase(REG0, 0.0);
    /*Инициализация дисплея*/
    disp.clear();
    disp.brightness(7);
    disp.point(0);    
    gen.SleepMode(true);    
}

/*Выполняем все по кругу*/
void loop(){
    /*Меняем частоту*/    
    if ((freq <= 999000) and (freq >= 30000))
    {
        freq = freq + 200;
        Disp = true;
        Serial.println(freq);
    }
    else
    {
        freq = 30000;
        Disp = true;
    }
      
    /*Если частота обновилась выводим на дисплей*/
    if (Disp){
        disp.clear();
        disp.displayInt(freq / 1000);
        Disp = false;
    }
    
    /*Крутим вертим фазу*/
    if (Phase == true)
    {
        
        Serial.println("true");
        gen.SetPhase(REG0, 0.0);
           gen.SetFrequency(REG0, freq);
           gen.SetOutputSource(REG0);
           Phase = false;
    }
    else
    {
        Serial.println("false");
        gen.SetPhase(REG0, 180);

            gen.SetFrequency(REG0, freq);
            gen.SetOutputSource(REG0);
            Phase = true;
    }
    gen.EnableOutput(true);
    delay(87);
}