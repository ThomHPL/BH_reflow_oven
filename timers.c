//FONCTIONS TIMER

//INCLUDE
#include "timers.h"
#include "main.h"

// callback timer2 init
void CB_TIMER_Init_1ms(void)
{
	TCCR2A |= (1<<WGM21);	
	TCCR2B |= (1<<CS22) | (0<<CS20);		// mode CTC, prescaler ./64
	// 8us / tick -> 125 tick = 1ms 
	
	OCR2A = 125;
	
	TIMSK2 |= (1<<OCIE2A);		
}

// PWM timer0 init
void PWM_Init(unsigned char DC)
{
	// clock 8000000 Hz prescaler 1024 : CS0=0b101
	// pwm frequency ~= 30 Hz
	// output on OC0A PD6
	// fast PWM : WGM = 0b011
	// non inverting mode: COM0A = 0b10
	
	sbiBF(DDRD,PD6); // OC0A as output
	TCCR0A |= (1<<COM0A1)|(1<<WGM01)|(1<<WGM00);
	// TCCR0B |= (1<<CS02)|(1<<CS00);					// disabled by default
	OCR0A = DC; // default: 0 %	
}

void PWM_setDutyCycle(unsigned char DC)
{
	OCR0A = DC;
}

void PWM_enable(void)
{
	TCCR0A |= (1<<COM0A1);
	TCCR0B |= (1<<CS02)|(1<<CS00);
	TCNT0 = 0x0;
}

void PWM_disable(void)
{
	TCCR0A &= ~(1<<COM0A1);
	TCCR0B &= ~((1<<CS02)|(1<<CS00));
	TCNT0=0xFF;
	cbiBF(PORTD,PD6);
	// fix forcing pin to 0!!
}