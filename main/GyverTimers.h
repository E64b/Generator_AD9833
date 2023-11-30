#define MAX_PERIOD_8 (1000000UL * 1024UL / F_CPU * 256UL)		// 16384 (61 Гц) на 16 МГц
#define MAX_PERIOD_16 (1000000UL * 1024UL / F_CPU * 65536UL)	// 4194304 (0.24 Гц) на 16 МГц

#ifndef GyverTimers_h
#define GyverTimers_h
#include <Arduino.h>

/* ==========  Константы ========== */
#define CHANNEL_A 0x00
#define CHANNEL_B 0x01
#define CHANNEL_C 0x02

#define TOGGLE_PIN 0x01
#define CLEAR_PIN 0x02
#define SET_PIN 0x03

#define TIMER0_A  TIMER0_COMPA_vect
#define TIMER0_B  TIMER0_COMPB_vect
#define TIMER1_A  TIMER1_COMPA_vect
#define TIMER1_B  TIMER1_COMPB_vect
#define TIMER2_A  TIMER2_COMPA_vect
#define TIMER2_B  TIMER2_COMPB_vect

#define GYVERTIMERS_INLINE
/*inline __attribute__((always_inline))*/

/* ================ Сlasses of timers ================ */
class Timer_0 {                       					  // Timer 0
public:
    uint32_t setPeriod(uint32_t _timer0_period);          // Set timer period [us]
    uint32_t setFrequency(uint32_t _timer0_frequency);    // Set timer frequency [Hz]
    float setFrequencyFloat(float _timer0_frequency);  	  // Set timer float frequency [Hz]
    
	GYVERTIMERS_INLINE
	bool ready(uint8_t channel = CHANNEL_A);			  // Return true, is interrupt is ready, but not enabled
	
    GYVERTIMERS_INLINE
    void enableISR(uint8_t source = CHANNEL_A);           // Enable timer interrupt , channel A or B 
    
    GYVERTIMERS_INLINE
    void disableISR(uint8_t source = CHANNEL_A);          // Disable timer interrupt , channel A or B
    
    GYVERTIMERS_INLINE
    void pause(void);                   				  // Disable timer clock , not cleaning the counter
    
    GYVERTIMERS_INLINE
    void resume(void);                 				      // Return clock timer settings
    
    GYVERTIMERS_INLINE
    void stop(void);                   					  // Disable timer clock , and cleaning the counter
    
    GYVERTIMERS_INLINE
    void restart(void);                  				  // Return clock timer settings & reset counter
    
    GYVERTIMERS_INLINE
    void setDefault(void);               			      // Set default timer settings
    
    GYVERTIMERS_INLINE
    void outputEnable(uint8_t channel, uint8_t mode);	  // Enable and configurate timer hardware output
    
    GYVERTIMERS_INLINE
    void outputDisable(uint8_t channel);				  // Disable timer hardware output
    
    GYVERTIMERS_INLINE
    void outputState(uint8_t channel,bool state);		  // Set High / Low on the timer output 
    
    GYVERTIMERS_INLINE
    void phaseShift(uint8_t source, uint16_t phase);
    
private:
    uint8_t _timer0_clock = 0x00;           			  // Variable to store timer clock settings
};

class Timer_1 {                      					  // Timer 1
public:
    uint32_t setPeriod(uint32_t _timer1_period);      	  // Set timer period [Hz]
    uint32_t setFrequency(uint32_t _timer1_frequency);    // Set timer frequency [Hz]
    float setFrequencyFloat(float _timer1_frequency);     // Set timer float frequency [Hz]
    
	GYVERTIMERS_INLINE
	bool ready(uint8_t channel = CHANNEL_A);			  // Return true, is interrupt is ready, but not enabled	
	
    GYVERTIMERS_INLINE
    void enableISR(uint8_t source = CHANNEL_A);       	  // Enable timer interrupt , channel A or B 
    
    GYVERTIMERS_INLINE
    void disableISR(uint8_t source = CHANNEL_A);          // Disable timer interrupt , channel A or B
    
    GYVERTIMERS_INLINE
    void pause(void);                   				  // Disable timer clock , not cleaning the counter
    
    GYVERTIMERS_INLINE
    void resume(void);                    				  // Return clock timer settings
    
    GYVERTIMERS_INLINE
    void stop(void);                    				  // Disable timer clock , and cleaning the counter	
    
    GYVERTIMERS_INLINE
    void restart(void);                   				  // Return clock timer settings & reset counter
    
    GYVERTIMERS_INLINE
    void setDefault(void);                  			  // Set default timer settings
    
    GYVERTIMERS_INLINE
    void outputEnable(uint8_t channel, uint8_t mode);	  // Enable and configurate timer hardware output
    
    GYVERTIMERS_INLINE
    void outputDisable(uint8_t channel);				  // Disable timer hardware output
    
    GYVERTIMERS_INLINE
    void outputState(uint8_t channel,bool state);		  // Set High / Low on the timer output  
    
    GYVERTIMERS_INLINE
    void phaseShift(uint8_t source, uint16_t phase);
    
private:
    uint8_t _timer1_clock = 0x00;             			  // Variable to store timer clock settings
};

class Timer_2 {                       					  // Timer 2
public:
    uint32_t setPeriod(uint32_t _timer2_period);      	  // Set timer period [Hz]
    uint32_t setFrequency(uint32_t _timer2_frequency);    // Set timer frequency [Hz]
    float setFrequencyFloat(float _timer2_frequency);     // Set timer float frequency [Hz]
    
	GYVERTIMERS_INLINE
	bool ready(uint8_t channel = CHANNEL_A);			  // Return true, is interrupt is ready, but not enabled
	
    GYVERTIMERS_INLINE
    void enableISR(uint8_t source = CHANNEL_A);      	  // Enable timer interrupt , channel A or B 
    
    GYVERTIMERS_INLINE
    void disableISR(uint8_t source = CHANNEL_A);          // Disable timer interrupt , channel A or B
    
    GYVERTIMERS_INLINE
    void pause(void);                   				  // Disable timer clock , not cleaning the counter
    
    GYVERTIMERS_INLINE
    void resume(void);                 				      // Return clock timer settings
    
    GYVERTIMERS_INLINE
    void stop(void);                    				  // Disable timer clock , and cleaning the counter
    
    GYVERTIMERS_INLINE
    void restart(void);                  				  // Return clock timer settings & reset counter
    
    GYVERTIMERS_INLINE
    void setDefault(void);                  			  // Set default timer settings
    
    GYVERTIMERS_INLINE
    void outputEnable(uint8_t channel, uint8_t mode);	  // Enable and configurate timer hardware output
    
    GYVERTIMERS_INLINE
    void outputDisable(uint8_t channel);				  // Disable timer hardware output
    
    GYVERTIMERS_INLINE
    void outputState(uint8_t channel,bool state);		  // Set High / Low on the timer output  
    
    GYVERTIMERS_INLINE
    void phaseShift(uint8_t source, uint16_t phase);
    
private:
    uint8_t _timer2_clock = 0x00;             			  // Variable to store timer clock settings	
};

extern Timer_0 Timer0;
extern Timer_1 Timer1;
extern Timer_2 Timer2;

#endif