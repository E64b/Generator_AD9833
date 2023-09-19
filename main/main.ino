#include "main.h"

void setup(){
    gen.Begin();
    disp.clear();
    disp.brightness(7); 
    disp.point(0);
    TCCR1A = TCCR1B = TCNT1 = cnt_ovf = 0;  // ����� �������
    TIFR1 |= (1 << TOV1);
    TIMSK1 = (1 << TOIE0);                  // ���������� ������������
    TCCR1B = (1 << CS10);                   // ����� �������
    TCCR1B = 0;                             // ���������� ������
    uint32_t count = TCNT1 - 2;             // ����� ��� ����� �� ��������
    count += ((uint32_t)cnt_ovf * 0xFFFF);  // � ������ ������������
  
}

ISR(TIMER1_OVF_vect)
{
    cnt_ovf++;
}

/*��������� �� ����� ��� �������*/
void loop(){
  CalcFreq();
  Display();
  SendSine();
  PeriodDelay();
}
