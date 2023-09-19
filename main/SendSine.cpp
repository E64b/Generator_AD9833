#include "main.h"

/*�������� �������� �������*/
void SendSine(){
    AD.setFrequency(freq, 0);
    AD.setWave(AD9833_SINE);
}
