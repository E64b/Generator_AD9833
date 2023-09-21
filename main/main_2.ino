/* Генератор 1 Hz..4 MHz. С регулировкой скважности. Энкодер
подключен к пинам A1 и A2, кнопка  энкодера подключена к A0.
Кнопка частота/скважность 2
Требуется использовать конденсаторы  0,01..0,1uf  относительно земли
на каждый вывод энкодера. Скетч для ардуино на мк atmega328 (UNO,Nano, MiniPro)
Дрова для 5110 взяты из  <a href="http://www.rinkydinkelectronics.com/library.php?id=44" title="http://www.rinkydinkelectronics.com/library.php?id=44" rel="nofollow">http://www.rinkydinkelectronics.com/library.php?id=44</a>
*/
#include <LCD5110_Basic.h>
LCD5110 lcd(3, 4, 5, 6, 7);
extern uint8_t SmallFont[];
extern uint8_t MediumNumbers[];
#define set_duty() { OCR1A=(uint32_t)ICR1*duty/100; }
uint8_t duty = 50;
float freq;
volatile uint8_t enc_mode_flag = 1; // По умолчанию энкодер регулирует частоту

void setup()
{
    lcd.InitLCD();
    lcd.setContrast(65);
    pinMode(13, OUTPUT); // LED
    pinMode(9, OUTPUT); // выход генератора PB2
    pinMode(A0, INPUT); // PC0 кнопка энкодера 
    pinMode(A1, INPUT); // PC1 вывод энкодера
    pinMode(A2, INPUT);  // PC2 вывод энкодера 
    pinMode(2, INPUT_PULLUP); // подтяжка кнопки "режим энкодера"
    PCICR = 1 << PCIE1; //разрешить прерывание PCINT1
    PCMSK1 = 1 << PCINT9;// По сигналу на А1 создавать прерывание
    TCCR1A = 1 << COM1A1; //подключить выход OC1A первого таймера
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
    uint8_t n = PINC & 6; //считать значение энкодера частоты
    if (enc_mode_flag)  enc_freq(n); // если энкодер в режиме частоты
    if (!enc_mode_flag) enc_duty(n); // если энкодер в режиме скважности
}


void enc_freq(uint8_t n)
{
    static boolean gen_mode = 0; //флаг режима управления
    static uint32_t enc = 1; //переменная счёта энкодера
    uint32_t icr = ICR1;
    uint16_t divider = 1; //переменная коэфф. деления прескалера
    boolean knopka = PINC & (1 << 0); // 0-кнопка нажата, 1-кнопка НЕ нажата.
    if (freq < 2848) gen_mode = 0; //переключение режима управления по частоте
    if (freq >= 2848) gen_mode = 1; //переключение режима управления по OCR
    // Если  увеличение частоты
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
        else knopka ? enc++ : enc += 100; // в нч режиме
    } //end GetUP

   // Если уменьшение частоты
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
    { //расчёт прескалера и ICR по нужной частоте
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
        //запись в регистр прескалера            
        switch (divider)
        {
        case 1: TCCR1B = 1 | (1 << WGM13); break;
        case 8: TCCR1B = 2 | (1 << WGM13); break;
        case 64: TCCR1B = 3 | (1 << WGM13); break;
        case 256: TCCR1B = 4 | (1 << WGM13); break;
        case 1024: TCCR1B = 5 | (1 << WGM13); break;
        }

        freq = (float)F_CPU / 2 / (ICR1 + 1) / divider;
    } //конец "если  не Gen_mode (частота менее 2848 герц)
} // конец enc_freq 


void enc_duty(uint8_t n)
{
    // Если  увеличение скважности
    if (n == 4 || n == 2)
    {
        if (duty < 100)
        {
            duty++;
        }
    }
    // Если уменьшение скважности
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
    delay(100);// что-б слишком часто в дисплей не писало..
}

