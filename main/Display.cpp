#include "main.h"
/*Выводим текущую частоту на дисплей*/
void Display()
{
    disp.clear();
    disp.displayInt(freq / 1000);
}