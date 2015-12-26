#ifndef __OS_H
#define __OS_H

#define	TIMER_CB_MAX		10
#include "main.h"
extern unsigned char OS_CURRENT_STATE;
extern BOOL OS_first_run;
void OS_init(void);
void OS_start(void);

unsigned char OS_stateMachine(char state, unsigned char stimuli);

unsigned char OS_addCallback(void(*ptFonction)(void), unsigned int period);
unsigned char OS_removeCallback(unsigned char CB_ID);
unsigned char OS_updateCallbackTime(unsigned char CB_ID,unsigned int time);



#endif //__OS_H
