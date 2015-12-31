#ifndef __OS_H
#define __OS_H

#include "main.h"

#define	TIMER_CB_MAX		10

extern unsigned char OS_CURRENT_STATE;
extern BOOL OS_first_run;

void OS_init(void);
void OS_start(void);

unsigned char OS_getNextState(char state, unsigned char stimuli);
unsigned char OS_addCallback(void(*ptFonction)(void), unsigned int period);
unsigned char OS_removeCallback(unsigned char CB_ID);
void OS_setCallbackPeriod(unsigned char CB_ID,unsigned int time);

#endif //__OS_H
