# Generator_SquareWave

√енератор ћеандра с частотой от 30к√ц до 999к√ц


float PWM_square_D9(float frequency) {
  TCCR1A = 1 << COM1A0;
  TCCR1C = 1 << FOC1A;
  if (frequency < (F_CPU / (65535UL * 16))) {
    TCCR1B = 1 << WGM12 | 1 << WGM13 | 0b011;
    ICR1 = ((float)F_CPU / (frequency * 128UL)) - 1;
    return ((float)F_CPU / ((ICR1 + 1) * 128.0f));
  } else if (frequency < (F_CPU / (65535UL * 2))) {
    TCCR1B = 1 << WGM12 | 1 << WGM13 | 0b010;
    ICR1 = ((float)F_CPU / (frequency * 16UL)) - 1;
    return ((float)F_CPU / ((ICR1 + 1) * 16.0f));
  } else {
    TCCR1B = 1 << WGM12 | 1 << WGM13 | 0b001;
    ICR1 = ((float)F_CPU / (frequency * 2UL)) - 1;
    return ((float)F_CPU / ((ICR1 + 1) * 2.0f));
  }
}
 
