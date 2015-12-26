/*
 * hardware.c
 *
 * Created: 30/10/2015 16:38:27
 *  Author: BOSTEM Antoine & HERPOEL Thomas
 */ 

#include "main.h"
#include "button.h"
#include "hardware.h"
#include "lcd.h"

void statusLed_init(void)
{
	sbiBF(DDRD,PIN_STATUS_LED);		// output Mode
	sbiBF(PORTD,PIN_STATUS_LED);	// ON at startup
}

void statusLedOn(void)
{
	sbiBF(PORTD,PIN_STATUS_LED);
}

void statusLedOff(void)
{
	cbiBF(PORTD,PIN_STATUS_LED);
}

void statusLedToggle(void)
{
	PIND = (1<<PIN_STATUS_LED);
}


