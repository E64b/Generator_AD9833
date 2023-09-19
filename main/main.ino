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
volatile uint16_t cnt_ovf = 0;
inline __attribute__((always_inline))

void test(void){
    asm("nop");
}


void setup(){
    gen.Begin();
    display.setBrightness(0x0f); // ��������� ������� �������  
     TCCR1A = TCCR1B = TCNT1 = cnt_ovf = 0;  // ����� �������
  TIFR1 |= (1 << TOV1);
  TIMSK1 = (1 << TOIE0);                  // ���������� ������������
  TCCR1B = (1 << CS10);                   // ����� �������
  test();                                 // ����
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
