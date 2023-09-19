#include "main.h"

/*Считаем период текущей частоты и устанавливаем паузу перед следующей*/
void PeriodDelay()
{
    int Timer = 0;
    AD.setWave(AD9833_OFF);
    periodns = round(1000000000 / freq); //Считаем период в наносекундах
    while (Timer < (periodns / 2))
    {
        TCCR1B = 0;                             // остановить таймер
        uint32_t count = TCNT1 - 2;             // минус два такта на действия
        count += ((uint32_t)cnt_ovf * 0xFFFF);  // с учетом переполнений
        Timer = Timer + (count * (float)(1000000000.0f / F_CPU), 0);
    }
}