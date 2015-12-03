#ifndef __OS_H
#define __OS_H

#define	TIMER_CB_MAX		10

extern unsigned char OS_CURRENT_STATE;

void OS_init(void);
void OS_start(void);
//void OS_refreshLcd(void);
unsigned char OS_stateMachine(char state, unsigned char stimuli);

unsigned char OS_addTimerCallback(void(*ptFonction)(void), unsigned int period);
unsigned char OS_removeTimerCallback(unsigned char CB_ID);
unsigned char OS_updateCallbackTime(unsigned char CB_ID,unsigned int time);



#endif //__OS_H
