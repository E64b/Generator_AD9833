#include "main.h"
/*Перебираем все частоты с шагом 200Гц, при достижении 999кГц возвращаемся обратно на 30кГц*/
void CalcFreq()
{
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