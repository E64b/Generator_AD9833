#include "main.h"
/*��� �� ������� ����� ����� ������ �������*/
void PeriodDelay()
{
    gen.EnableOutput(false);
    periodns = round(1000000000 / freq); //������� ������ � ������������ � ��������� �� ������ �����
    TCCR1A = TCCR1B = TCNT1 = cnt_ovf = 0;  // ����� �������
    while (Timer < (periodns / 2)){        
        TIFR1 |= (1 << TOV1);
        TIMSK1 = (1 << TOIE0);                  // ���������� ������������
        TCCR1B = (1 << CS10);                   // ����� �������
        int Timer = count * (1000000000 / F_CPU), 4) //�������� ����� � ������������
        TCCR1B = 0;                             // ���������� ������
        uint32_t count = TCNT1 - 2;             // ����� ��� ����� �� ��������
        count += ((uint32_t)cnt_ovf * 0xFFFF);  // � ������ ������������
    }
}