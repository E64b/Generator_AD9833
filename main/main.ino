#include <SPI.h>
#include <TM1637Display.h> // ����������� ���������� ��� ���������� �������� TM1637 
#include <AD9833.h>
#include <Arduino.h>

#define FNC_PIN 4
#define CLK 2 // ����������� CLK ��� ����������� ������� TM1637 
#define DIO 3 // ����������� DIO ��� ����������� ������� TM1637 

AD9833 gen(FNC_PIN);
TM1637Display display(CLK, DIO);

uint16_t freq = 30000;
uint32_t period = 0;

void setup(){
    gen.Begin();
    display.setBrightness(0x0f); // ��������� ������� �������   
    pinMode(3, OUTPUT);
}


/*��� �� ������� ����� ����� ������ �������*/
void PeriodDelay(){
    gen.EnableOutput(false);
    periodns = round(1000000000 / freq); //������� ������ � ������������ � ��������� �� ������ �����
    while (Timer2 < (period / 2))
    {
    }
}

/*��� �� ������� �� ����� ������� �������*/
void Display(){
    display.showNumberDec(freq / 1000);
}

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

/*�������� �������� �������*/
void SendSine(){
    gen.ApplySignal(SINE_WAVE, REG0, freq);
    gen.EnableOutput(true);
}

/*��������� �� ����� ��� �������*/
void loop(){
  CalcFreq();
  Display();
  SendSine();
  PeriodDelay();
}
