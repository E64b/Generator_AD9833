#include "GyverTimers.h"

// ========================== READY ==========================
bool Timer_0::ready(uint8_t channel) {
  bool flag = false;
  switch (channel) {
  case CHANNEL_A:
    flag = bitRead(TIFR0, OCF0A);
    if (flag)
      bitSet(TIFR0, OCF0A);
    break; // Return interrupt flag + clear flag
  case CHANNEL_B:
    flag = bitRead(TIFR0, OCF0B);
    if (flag)
      bitSet(TIFR0, OCF0B);
    break;
  }
  return flag;
}

bool Timer_1::ready(uint8_t channel) {
  bool flag = false;
  switch (channel) {
  case CHANNEL_A:
    flag = bitRead(TIFR1, OCF1A);
    if (flag)
      bitSet(TIFR1, OCF1A);
    break; // Return interrupt flag + clear flag
  case CHANNEL_B:
    flag = bitRead(TIFR1, OCF1B);
    if (flag)
      bitSet(TIFR1, OCF1B);
    break;
  }
  return flag;
}

bool Timer_2::ready(uint8_t channel) {
  bool flag = false;
  switch (channel) {
  case CHANNEL_A:
    flag = bitRead(TIFR2, OCF2A);
    if (flag)
      bitSet(TIFR2, OCF2A);
    break; // Return interrupt flag + clear flag
  case CHANNEL_B:
    flag = bitRead(TIFR2, OCF2B);
    if (flag)
      bitSet(TIFR2, OCF2B);
    break;
  }
  return flag;
}

// ========================== OUTPUT STATE ==========================
void Timer_0::outputState(uint8_t channel, bool state) {
  switch (channel) {
  case CHANNEL_A:
    TCCR0B = (TCCR0B & 0x7F) | (state << FOC0A);
    break;
  case CHANNEL_B:
    TCCR0B = (TCCR0B & 0xBF) | (state << FOC0B);
    break;
  }
}

void Timer_1::outputState(uint8_t channel, bool state) {
  switch (channel) {
  case CHANNEL_A:
    TCCR1C = (TCCR1C & 0x7F) | (state << FOC1A);
    break;
  case CHANNEL_B:
    TCCR1C = (TCCR1C & 0xBF) | (state << FOC1B);
    break;
  }
}

void Timer_2::outputState(uint8_t channel, bool state) {
  switch (channel) {
  case CHANNEL_A:
    TCCR2B = (TCCR2B & 0x7F) | (state << FOC2A);
    break;
  case CHANNEL_B:
    TCCR2B = (TCCR2B & 0xBF) | (state << FOC2B);
    break;
  }
}

// ========================== PAUSE ==========================
void Timer_0::pause(void) {
  TCCR0B = (TCCR0B & 0xF8); // Clear timer clock bits
}

void Timer_1::pause(void) { TCCR1B = (TCCR1B & 0xF8); }

void Timer_2::pause(void) { TCCR2B = (TCCR2B & 0xF8); }

// ========================== RESUME ==========================
void Timer_0::resume(void) {
  TCCR0B = ((TCCR0B & 0xF8) | _timer0_clock); // Return clock timer settings
}

void Timer_1::resume(void) { TCCR1B = ((TCCR1B & 0xF8) | _timer1_clock); }

void Timer_2::resume(void) { TCCR2B = ((TCCR2B & 0xF8) | _timer2_clock); }

// ========================== STOP ==========================
void Timer_0::stop(void) {
  Timer_0::pause();
  TCNT0 = 0x00; // Clear timer counter
}

void Timer_1::stop(void) {
  Timer_1::pause();
  TCNT1 = 0x00;
}

void Timer_2::stop(void) {
  Timer_2::pause();
  TCNT2 = 0x00;
}

// ========================== RESTART ==========================
void Timer_0::restart(void) {
  Timer_0::resume();
  TCNT0 = 0x00;
}

void Timer_1::restart(void) {
  Timer_1::resume();
  TCNT1 = 0x00;
}

void Timer_2::restart(void) {
  Timer_2::resume();
  TCNT2 = 0x00;
}

// ========================== DISABLE ISR ==========================
void Timer_0::disableISR(uint8_t source) {
  TIMSK0 &= ~(source ? (1 << OCIE0B) : (1 << OCIE0A));
}

void Timer_1::disableISR(uint8_t source) {
  switch (source) {
  case CHANNEL_A:
    TIMSK1 &= ~(1 << OCIE1A);
    break;
  case CHANNEL_B:
    TIMSK1 &= ~(1 << OCIE1B);
    break;
  }
}

void Timer_2::disableISR(uint8_t source) {
  TIMSK2 &= ~(source ? (1 << OCIE2B) : (1 << OCIE2A));
}

// ========================== DEFAULT ==========================
void Timer_0::setDefault(void) {
  TCCR0A = 0x03; // Fast PWM , 8 bit
  TCCR0B = 0x03; // Prescaler /64
  OCR0B = 0x00;  // Clear COMPA
  OCR0A = 0x00;  // Clear COMPB
  TCNT0 = 0x00;  // Clear counter
}

void Timer_1::setDefault(void) {
  TCCR1A = 0x01; // Phasecorrect PWM , 8 bit
  TCCR1B = 0x0B; // Prescaler /64
  OCR1B = 0x00;  // Clear COMPA
  OCR1A = 0x00;  // Clear COMPB
  TCNT1 = 0x00;  // Clear counter
}

void Timer_2::setDefault(void) {
  TCCR2A = 0x01; // Phasecorrect PWM , 8 bit
  TCCR2B = 0x04; // Prescaler /64
  OCR2B = 0x00;  // Clear COMPA
  OCR2A = 0x00;  // Clear COMPB
  TCNT2 = 0x00;  // Clear counter
}

// ========================== PHASE SHIFT ==========================
void Timer_0::phaseShift(uint8_t source, uint16_t phase) {
  if (source)
    OCR0B = map(phase, 0, 360, 0, OCR0A);
}

void Timer_1::phaseShift(uint8_t source, uint16_t phase) {
  switch (source) {
  case CHANNEL_A:
    OCR1A = map(phase, 0, 360, 0, ICR1);
    break;
  case CHANNEL_B:
    OCR1B = map(phase, 0, 360, 0, ICR1);
    break;
  }
}

void Timer_2::phaseShift(uint8_t source, uint16_t phase) {
  if (source)
    OCR2B = map(phase, 0, 360, 0, OCR2A);
}

// ========================== ENABLE ISR ==========================
void Timer_0::enableISR(uint8_t source) {
  if (!source)
    TIMSK0 |= (1 << OCIE0A);
  else
    TIMSK0 |= (1 << OCIE0B);
}

void Timer_1::enableISR(uint8_t source) {
  switch (source) {
  case CHANNEL_A:
    TIMSK1 |= (1 << OCIE1A);
    break;
  case CHANNEL_B:
    TIMSK1 |= (1 << OCIE1B);
    break;
  }
}

void Timer_2::enableISR(uint8_t source) {
  if (!source)
    TIMSK2 |= (1 << OCIE2A);
  else
    TIMSK2 |= (1 << OCIE2B);
}

// ========================== SET FREQUENCY ==========================
uint32_t Timer_0::setFrequency(uint32_t _timer0_frequency) {
  return 1000000UL / (Timer_0::setPeriod(1000000UL / _timer0_frequency));
}

uint32_t Timer_1::setFrequency(uint32_t _timer1_frequency) {
  return 1000000UL / (Timer_1::setPeriod(1000000UL / _timer1_frequency));
}

uint32_t Timer_2::setFrequency(uint32_t _timer2_frequency) {
  return 1000000UL / (Timer_2::setPeriod(1000000UL / _timer2_frequency));
}

// ========================== SET FREQUENCY FLOAT ==========================
float Timer_0::setFrequencyFloat(float _timer0_frequency) {
  return 1000000.0F / (Timer_0::setPeriod(1000000.0F / _timer0_frequency));
}

float Timer_1::setFrequencyFloat(float _timer1_frequency) {
  return 1000000.0F / (Timer_1::setPeriod(1000000.0F / _timer1_frequency));
}

float Timer_2::setFrequencyFloat(float _timer2_frequency) {
  return 1000000.0F / (Timer_2::setPeriod(1000000.0F / _timer2_frequency));
}

// ========================== OUTPUT ENABLE ==========================
void Timer_0::outputEnable(uint8_t channel, uint8_t mode) {
  switch (channel) {
  case CHANNEL_A:
    TCCR0A = (TCCR0A & 0x3F) | (mode << 6);
    break;
  case CHANNEL_B:
    TCCR0A = (TCCR0A & 0xCF) | (mode << 4);
    break;
  }
}

void Timer_1::outputEnable(uint8_t channel, uint8_t mode) {
  switch (channel) {
  case CHANNEL_A:
    TCCR1A = (TCCR1A & 0x3F) | (mode << 6);
    break;
  case CHANNEL_B:
    TCCR1A = (TCCR1A & 0xCF) | (mode << 4);
    break;
  }
}

void Timer_2::outputEnable(uint8_t channel, uint8_t mode) {
  switch (channel) {
  case CHANNEL_A:
    TCCR2A = (TCCR2A & 0x3F) | (mode << 6);
    break;
  case CHANNEL_B:
    TCCR2A = (TCCR2A & 0xCF) | (mode << 4);
    break;
  }
}

// ========================== OUTPUT DISABLE ==========================
void Timer_0::outputDisable(uint8_t channel) {
  switch (channel) {
  case CHANNEL_A:
    TCCR0A = (TCCR0A & 0x3F);
    break;
  case CHANNEL_B:
    TCCR0A = (TCCR0A & 0xCF);
    break;
  }
}

void Timer_1::outputDisable(uint8_t channel) {
  switch (channel) {
  case CHANNEL_A:
    TCCR1A = (TCCR1A & 0x3F);
    break;
  case CHANNEL_B:
    TCCR1A = (TCCR1A & 0xCF);
    break;
  }
}

void Timer_2::outputDisable(uint8_t channel) {
  switch (channel) {
  case CHANNEL_A:
    TCCR2A = (TCCR2A & 0x3F);
    break;
  case CHANNEL_B:
    TCCR2A = (TCCR2A & 0xCF);
    break;
  }
}

// ========================== SET PERIOD ==========================
uint32_t Timer_0::setPeriod(uint32_t _timer0_period) {
  _timer0_period = constrain(_timer0_period, 1, MAX_PERIOD_8);

  uint32_t _timer0_cycles =
      F_CPU / 1000000 *
      _timer0_period; // Calculation of the number of timer cycles per period
  uint8_t _timer0_prescaler = 0x00;
  uint16_t _timer0_divider = 0x00;

  if (_timer0_cycles < 256UL) { // Сhoose optimal divider for the timer
    _timer0_prescaler = 0x01;
    _timer0_divider = 1UL;
  } else if (_timer0_cycles < 256UL * 8) {
    _timer0_prescaler = 0x02;
    _timer0_divider = 8UL;
  } else if (_timer0_cycles < 256UL * 64) {
    _timer0_prescaler = 0x03;
    _timer0_divider = 64UL;
  } else if (_timer0_cycles < 256UL * 256) {
    _timer0_prescaler = 0x04;
    _timer0_divider = 256UL;
  } else {
    _timer0_prescaler = 0x05;
    _timer0_divider = 1024UL;
  }

  uint8_t _timer0_top =
      (_timer0_cycles < 256UL * 1024 ? (_timer0_cycles / _timer0_divider)
                                     : 256UL);

  TCCR0A = (TCCR0A & 0xF0) | (1 << WGM21); // CTC - mode
  TCCR0B = _timer0_prescaler;              // Set timer prescaler
  OCR0A = _timer0_top - 1;                 // Set timer top
  _timer0_clock = (TCCR0B & 0x07);         // Save timer clock settings

  return (1000000UL / ((F_CPU / _timer0_divider) /
                       _timer0_top)); // Return real timer period
}

uint32_t Timer_1::setPeriod(uint32_t _timer1_period) {
  _timer1_period = constrain(_timer1_period, 1, MAX_PERIOD_16);

  uint32_t _timer1_cycles =
      F_CPU / 1000000 *
      _timer1_period; // Calculation of the number of timer cycles per period
  uint8_t _timer1_prescaler = 0x00;
  uint16_t _timer1_divider = 0x00;

  if (_timer1_cycles < 65536UL) { // Сhoose optimal divider for the timer
    _timer1_prescaler = 0x01;
    _timer1_divider = 1UL;
  } else if (_timer1_cycles < 65536UL * 8) {
    _timer1_prescaler = 0x02;
    _timer1_divider = 8UL;
  } else if (_timer1_cycles < 65536UL * 64) {
    _timer1_prescaler = 0x03;
    _timer1_divider = 64UL;
  } else if (_timer1_cycles < 65536UL * 256) {
    _timer1_prescaler = 0x04;
    _timer1_divider = 256UL;
  } else {
    _timer1_prescaler = 0x05;
    _timer1_divider = 1024UL;
  }

  uint16_t _timer1_top =
      (_timer1_cycles < 65536UL * 1024 ? (_timer1_cycles / _timer1_divider)
                                       : 65536UL);
#if defined(__AVR_ATmega2560__)
  TCCR1A = (TCCR1A & 0xFC);
#else
  TCCR1A = (TCCR1A & 0xF0);
#endif
  TCCR1B = ((1 << WGM13) | (1 << WGM12) |
            _timer1_prescaler);    // CTC mode + set prescaler
  ICR1 = _timer1_top - 1;          // Set timer top
  _timer1_clock = (TCCR1B & 0x07); // Save timer clock settings
  return (1000000UL / ((F_CPU / _timer1_divider) /
                       _timer1_top)); // Return real timer period
}

uint32_t Timer_2::setPeriod(uint32_t _timer2_period) {
  _timer2_period = constrain(_timer2_period, 1, MAX_PERIOD_8);

  uint32_t _timer2_cycles =
      F_CPU / 1000000 *
      _timer2_period; // Calculation of the number of timer cycles per period
  uint8_t _timer2_prescaler = 0x00;
  uint16_t _timer2_divider = 0x00;

  if (_timer2_cycles < 256UL) { // Сhoose optimal divider for the timer
    _timer2_prescaler = 0x01;
    _timer2_divider = 1UL;
  } else if (_timer2_cycles < 256UL * 8) {
    _timer2_prescaler = 0x02;
    _timer2_divider = 8UL;
  } else if (_timer2_cycles < 256UL * 32) {
    _timer2_prescaler = 0x03;
    _timer2_divider = 32UL;
  } else if (_timer2_cycles < 256UL * 64) {
    _timer2_prescaler = 0x04;
    _timer2_divider = 64UL;
  } else if (_timer2_cycles < 256UL * 128) {
    _timer2_prescaler = 0x05;
    _timer2_divider = 128UL;
  } else if (_timer2_cycles < 256UL * 256) {
    _timer2_prescaler = 0x06;
    _timer2_divider = 256UL;
  } else {
    _timer2_prescaler = 0x07;
    _timer2_divider = 1024UL;
  }

  uint8_t _timer2_top =
      (_timer2_cycles < 256UL * 1024 ? (_timer2_cycles / _timer2_divider)
                                     : 256UL);

  TCCR2A = (TCCR2A & 0xF0) | (1 << WGM21); // CTC - mode
  TCCR2B = _timer2_prescaler;              // Set timer prescaler
  OCR2A = _timer2_top - 1;                 // Set timer top
  _timer2_clock = (TCCR2B & 0x07);         // Save timer clock settings

  return (1000000UL / ((F_CPU / _timer2_divider) /
                       _timer2_top)); // Return real timer period
}

Timer_0 Timer0 = Timer_0();
Timer_1 Timer1 = Timer_1();
Timer_2 Timer2 = Timer_2();
