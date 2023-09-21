/* ��������� 1 Hz..4 MHz. � ������������ ����������. �������
��������� � ����� A1 � A2, ������  �������� ���������� � A0.
������ �������/���������� 2
��������� ������������ ������������  0,01..0,1uf  ������������ �����
�� ������ ����� ��������. ����� ��� ������� �� �� atmega328 (UNO,Nano, MiniPro)
����� ��� 5110 ����� ��  <a href="http://www.rinkydinkelectronics.com/library.php?id=44" title="http://www.rinkydinkelectronics.com/library.php?id=44" rel="nofollow">http://www.rinkydinkelectronics.com/library.php?id=44</a>
*/
#include <LCD5110_Basic.h>
LCD5110 lcd(3, 4, 5, 6, 7);
extern uint8_t SmallFont[];
extern uint8_t MediumNumbers[];
#define set_duty() { OCR1A=(uint32_t)ICR1*duty/100; }
uint8_t duty = 50;
float freq;
volatile uint8_t enc_mode_flag = 1; // �� ��������� ������� ���������� �������

void setup()
{
    lcd.InitLCD();
    lcd.setContrast(65);
    pinMode(13, OUTPUT); // LED
    pinMode(9, OUTPUT); // ����� ���������� PB2
    pinMode(A0, INPUT); // PC0 ������ �������� 
    pinMode(A1, INPUT); // PC1 ����� ��������
    pinMode(A2, INPUT);  // PC2 ����� �������� 
    pinMode(2, INPUT_PULLUP); // �������� ������ "����� ��������"
    PCICR = 1 << PCIE1; //��������� ���������� PCINT1
    PCMSK1 = 1 << PCINT9;// �� ������� �� �1 ��������� ����������
    TCCR1A = 1 << COM1A1; //���������� ����� OC1A ������� �������
    TCCR1B = 0;//
    EICRA = 1 << ISC01; //int0 -falling mode
    EIMSK = 1; //int0 enable
}

ISR(INT0_vect)
{
    if ((PIND & (1 << 2)) == 0)
    {
        enc_mode_flag = !enc_mode_flag;
    }
}
ISR(PCINT1_vect)
{
    ;
    uint8_t n = PINC & 6; //������� �������� �������� �������
    if (enc_mode_flag)  enc_freq(n); // ���� ������� � ������ �������
    if (!enc_mode_flag) enc_duty(n); // ���� ������� � ������ ����������
}


void enc_freq(uint8_t n)
{
    static boolean gen_mode = 0; //���� ������ ����������
    static uint32_t enc = 1; //���������� ����� ��������
    uint32_t icr = ICR1;
    uint16_t divider = 1; //���������� �����. ������� ����������
    boolean knopka = PINC & (1 << 0); // 0-������ ������, 1-������ �� ������.
    if (freq < 2848) gen_mode = 0; //������������ ������ ���������� �� �������
    if (freq >= 2848) gen_mode = 1; //������������ ������ ���������� �� OCR
    // ����  ���������� �������
    if (n == 4 || n == 2)
    {
        if (gen_mode)
        {
            if (knopka)
            {
                if (icr > 2)
                {
                    icr--;
                }
            }
            else
            {
                if (icr > 12)icr -= 10;
            }
        }
        else knopka ? enc++ : enc += 100; // � �� ������
    } //end GetUP

   // ���� ���������� �������
    if (n == 6 || n == 0)
    {
        if (gen_mode)
        {
            if (knopka)
            {
                if (icr < 65535)
                {
                    icr++;
                }
            }
            else
            {
                if (icr <= 65525)icr += 10;
            }
        }
        else
        {
            if (knopka)
            {
                if (enc >= 2)enc--;
            }
            else
            {
                if (enc > 100) enc -= 100;
            }
        }
    } //end GetDown


    if (gen_mode)
    {
        ICR1 = icr; set_duty(); freq = (float)F_CPU / 2 / ICR1;
    }
    else
    { //������ ���������� � ICR �� ������ �������
        divider = 1; icr = F_CPU / enc / 2 / divider;
        if (icr > 65536)
        {
            divider = 8; icr = F_CPU / enc / 2 / divider;
            if (icr > 65536)
            {
                divider = 64; icr = F_CPU / enc / 2 / divider;
                if (icr > 65536)
                {
                    divider = 256; icr = F_CPU / enc / 2 / divider;
                    if (icr > 65536)
                    {
                        divider = 1024; icr = F_CPU / enc / 2 / divider;
                        if (icr > 65536)
                        {
                            icr = 65536;
                        }
                    }
                }
            }
        } ICR1 = icr - 1; set_duty();
        //������ � ������� ����������            
        switch (divider)
        {
        case 1: TCCR1B = 1 | (1 << WGM13); break;
        case 8: TCCR1B = 2 | (1 << WGM13); break;
        case 64: TCCR1B = 3 | (1 << WGM13); break;
        case 256: TCCR1B = 4 | (1 << WGM13); break;
        case 1024: TCCR1B = 5 | (1 << WGM13); break;
        }

        freq = (float)F_CPU / 2 / (ICR1 + 1) / divider;
    } //����� "����  �� Gen_mode (������� ����� 2848 ����)
} // ����� enc_freq 


void enc_duty(uint8_t n)
{
    // ����  ���������� ����������
    if (n == 4 || n == 2)
    {
        if (duty < 100)
        {
            duty++;
        }
    }
    // ���� ���������� ����������
    if (n == 6 || n == 0)
    {
        if (duty > 0)
        {
            duty--;
        }
    }
    set_duty();
}

void loop()
{
    String freqstr, dutystr, modestr;

    if (freq < 10000)
    {
        freqstr = String(String(freq, 2) + " Hz        ");
    }
    if (freq > 10000)
    {
        freqstr = String(String((freq / 1000), 3) + " KHz       ");
    }
    lcd.setFont(SmallFont);
    //byte l= freqstr.length();
    lcd.print(freqstr, LEFT, 0);

    dutystr = String("Duty=" + String(duty) + " %        ");
    lcd.print(dutystr, LEFT, 8);

    enc_mode_flag ? modestr = String("ENCMode=Freq") : modestr = String("ENCMode=Duty");
    lcd.print(modestr, LEFT, 16);
    delay(100);// ���-� ������� ����� � ������� �� ������..
}

