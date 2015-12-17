/*
 * PID.c
 *
 * Created: 16/12/2015 19:09:47
 *  Author: Thomas
 */ 

// PID type mixte
// commande =  [0..255]

#include "OS.h"
#include "PID.h"

unsigned int PID_K;
unsigned int PID_Ti;
unsigned int PID_Td;
unsigned int PID_consigne;	// en °C
unsigned int PID_period;	// en ms
int PID_command;

int lastError;
int lastInt;

unsigned char CBID_PID	= 0;

void PID_start(unsigned int consigne,unsigned int periode)
{
	PID_consigne=consigne;
	PID_period=periode;
	lastError=consigne-MAX_getTemp();
	lastInt=0;
	PID_command = 0;
	PWM_Init(0);
	PWM_enable();
	CBID_PID	= OS_addTimerCallback(PID_routine,PID_period);
}

void PID_stop()
{
	CBID_PID	= OS_removeTimerCallback(CBID_PID);
}

void PID_setParams(unsigned int K,unsigned int Ti,unsigned int Td)
{
	PID_K=K;
	PID_Ti=Ti;
	PID_Td=Td;
}

void PID_routine()
{
	PID_compute();
	PID_updateOutput();
	char msg[16];
	sprintf(msg,"Command: %d",PID_command);
	RS232_println(msg);
}

void PID_compute()
{
	unsigned int currentValue	= MAX_getTemp();
	int currentError	= PID_consigne-currentValue;
	int currentInt		= lastInt + PID_period*currentError;		// implementer Tustin??
	int currentDer		= (currentError-lastError)/PID_period;
	
	PID_command = 0;
	int integrale = 0;
	int derivee =0;
	
	
	// gain K
	PID_command = PID_K*currentError;
	
	// 1/pTi	:	integrateur
	integrale = PID_command * currentInt * 1/PID_Ti;
	
	// p*Td
	derivee = currentDer * PID_Td;
	
	// sommateur
	
	int estimation = PID_command + integrale + derivee;
	if(estimation >= 255 || estimation <= 0)	// anti saturation
	{
		PID_command = PID_command + derivee;
	}
	else
		PID_command = PID_command + integrale + derivee;
		
	// saturation
	if(PID_command>255)
		PID_command=255;
	else if(PID_command<0)
		PID_command=0;
}
void PID_updateOutput()
{
	PWM_setDutyCycle((unsigned char)(PID_command));
}