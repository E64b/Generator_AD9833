#include "main.h"

extern GyverTM1637 disp(CLK, DIO);

void setup(){
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

ISR(TIMER1_OVF_vect){
    cnt_ovf++;
}

/*Выполняем все по кругу*/
void loop()
{
  CalcFreq();
  Display();
  SendSine();
  PeriodDelay();
}
