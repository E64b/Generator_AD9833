#include "main.h"
/*����������� ������� � ����� 200��, ���� ������� ���� 999��� ������������ �� 30���*/
void CalcFreq(){
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