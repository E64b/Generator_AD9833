/*Тут мы считаем паузу между сменой частоты*/
void PeriodDelay(){
    gen.EnableOutput(false);
    periodns = round(1000000000 / freq); //считаем период в наносекундах и округляем до целого числа
    TCCR1A = TCCR1B = TCNT1 = cnt_ovf = 0;  // Сброс таймера
    while (Timer < (period / 2)){        
        TIFR1 |= (1 << TOV1);
        TIMSK1 = (1 << TOIE0);                  // Прерывание переполнения
        TCCR1B = (1 << CS10);                   // Старт таймера
        int Timer = count * (int)(1000000000 / F_CPU), 4) //Получаем время в наносекундах
        TCCR1B = 0;                             // остановить таймер
        uint32_t count = TCNT1 - 2;             // минус два такта на действия
        count += ((uint32_t)cnt_ovf * 0xFFFF);  // с учетом переполнений
    }
}