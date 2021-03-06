/*
 * PID.c
 *
 * Created: 16/12/2015 19:09:47
 *  Author: BOSTEM Antoine & HERPOEL Thomas
 */ 

// PID type mixte
// commande =  [0..255]

#include "OS.h"
#include "PID.h"
#include "RS232.h"
#include "timers.h"
#include "MAX.h"

float PID_K;
float PID_Ti;
float PID_Td;
int PID_consigne;	// en �C
int PID_period = 5000;	// en ms
float PID_command;


float lastE=0;
float intE=0;

unsigned char CBID_PID	= 0;

void PID_start()
{
	lastE=(float)(PID_consigne-MAX_getTemp());
	intE=0;
	PID_command = 0;
	PWM_Init(0);
	PWM_enable();
	CBID_PID	= OS_addCallback(PID_routine,PID_period);
	RS232_println("T;cmd;e;intE;dE");
}

void PID_stop()
{
	PWM_disable();
	CBID_PID	= OS_removeCallback(CBID_PID);
}

void PID_setConsigne(int consigne)
{
	PID_consigne=consigne;
}

void PID_setParams(float K,float Ti,float Td)
{
	PID_K=K;
	PID_Ti=Ti;
	PID_Td=Td;
}

void PID_routine()
{
	PID_compute();
	PID_updateOutput();
}

void PID_compute()
{

	float e = (float)(PID_consigne-MAX_getTemp());
	intE+=e*(float)PID_period/1000.0;
	float dE=(e-lastE)*1000.0/(float)PID_period;
	
	PID_command = 0;

	// sommateur
	
	float estimation = PID_K*(e + intE/PID_Ti + dE*PID_Td);
	if(estimation >= 255 || estimation <= 0)	// anti saturation
	{
		intE-=e*(float)PID_period/1000.0; // si sat, on retire l'int�grale qu'on vient de mesurer
		PID_command = PID_K*(e + intE/PID_Ti + dE*PID_Td);
	}
	else
		PID_command = PID_K*(e + intE/PID_Ti + dE*PID_Td);
		
	// saturation
	if(PID_command>255.0)
		PID_command=255;
	else if(PID_command<0.0)
		PID_command=0;
	
	//
	char msg[64];
	sprintf(msg,"%d;%d;%d;%d;%d",MAX_getTemp(),(int)PID_command,(int)e,(int)intE,(int)dE);
	RS232_println(msg);
	lastE=e;

}

void PID_updateOutput()
{
	PWM_setDutyCycle((unsigned char)(PID_command));
}