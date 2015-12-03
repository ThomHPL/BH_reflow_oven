// TIMERS.h

#ifndef _TIMERS_H_
#define _TIMERS_H_

//PROTOTYPE FONCTIONS EXTERNES
void CB_TIMER_Init_1ms(void);
void PWM_Init(unsigned char DC);
void PWM_setDutyCycle(unsigned char DC);
void PWM_enable(void);
void PWM_disable(void);


#endif /* _TIMERS_H */
