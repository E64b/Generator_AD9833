#include <GyverTM1637.h>
#include "AD9833.h"
#include <SPI.h>

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
    gen.SetPhase(REG0, 0);
    gen.SetPhase(REG1, 180);
    gen.EnableOutput(true);
    /*Инициализация дисплея*/
    disp.clear();
    disp.brightness(7);
    disp.point(0);     
}

/*Выполняем все по кругу*/
void loop(){
    delay(87);
    /*Меняем частоту*/    
    if ((freq <= 999000) and (freq >= 30000))
    {
        freq = freq + 200;
        Disp = true;
        Serial.println(freq);
        gen.SetFrequency(REG0, freq);
        gen.SetFrequency(REG1, freq);
        
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
        Phase = false;
        gen.SetOutputSource(REG0);
    }
    else
    {
        Serial.println("false");       
        Phase = true;
        gen.SetOutputSource(REG1);
    }
         
}