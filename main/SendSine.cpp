/*Начинаем передачу сигнала*/
void SendSine()
{
    gen.ApplySignal(SINE_WAVE, REG0, freq);
    gen.EnableOutput(true);
}
