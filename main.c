/*
 * main.c
 *
 * Created: 30/10/2015 14:29:45
 *  Author: Thomas & Antoine
 */ 


//#include <stdint.h>

#include "main.h"
#include "button.h"
#include "hardware.h"
#include "lcd.h"
#include "OS.h"
#include "menu.h"
#include "RS232.h"
#include "MAX.h"
#include "SPI.h"
#include "timers.h"
#include "I2C.h"
#include "EEPROM.h"
#include "RTC.h"
#include "PID.h"

char CBID_TglLed = 0;
char CBID_MAXroutine = 0;
char CBID_RefreshLCD = 0;
char CBID_DataLogger = 0;

void delay(int ms);
void delaycb(void);
void init(void);
void dataLogger(void);

int	tempPalier = 150;
int tempPic = 250;

int blinkPeriod=1000;
unsigned char PWM_DC = 127;

// MAIN
int main(void)
{	
	
	lcd_init(LCD_DISP_ON);
	
	OS_init();

	RS232_init(9600);	
	
	statusLed_init();

	KEYBOARD_init();
	
	MAX_init();
	
	I2C_init(100);
	
	sei();
	
	// header
	RS232_print("\r\n");
	RS232_print("BH REFLOW OVEN V00.00.01");
	RS232_print("\r\n\r\n\0");
	
	PWM_Init(PWM_DC);
	
	CBID_TglLed	= OS_addCallback(statusLedToggle,blinkPeriod);
	CBID_MAXroutine = OS_addCallback(MAX_start,20);				// launch a measure every 20 ms

	OS_start();
		
	return 1;
}

// main menu
char st_welcome(char input)
{
	static BOOL first_run = TRUE;
	static int32_t oldTmp=0;
	int32_t tmp = 0;
	tmp = MAX_getTemp();
	
	if(first_run==TRUE)
	{
		first_run=FALSE;
	}		
	else if(tmp!=oldTmp)
	{
		char msg[16];
		
		cli();
		lcd_clrscr();
		lcd_gotoxy(0,0);
		lcd_puts_p(ST_WELCOME_TXT);
		lcd_gotoxy(0,1);
		sprintf(msg,"%d 'C",(int)tmp);
		lcd_puts(msg);
		sei();
		oldTmp=tmp;
	}
	
	unsigned char nextstate = OS_stateMachine(OS_CURRENT_STATE, input);
	if (nextstate!=OS_CURRENT_STATE) // Si on quitte l'état
		first_run=TRUE;
	return nextstate;
}

char st_temp_palier(char input)
{
	static BOOL first_run = TRUE;
	if(first_run==TRUE)
	{
		char msg[16];
		sprintf(msg,"%d",tempPalier);
		cli();
		lcd_clrscr();
		lcd_gotoxy(0,0);
		lcd_puts_p(ST_TEMP_SETPALIER_TXT);
		lcd_gotoxy(0,1);
		lcd_puts(msg);
		sei();
		first_run=FALSE;
	}
	else if(input==KEY_UP && tempPalier<=250)
	{
		tempPalier+=5;
		char msg[16];
		sprintf(msg,"%d",tempPalier);
		cli();
		lcd_clrscr();
		lcd_gotoxy(0,0);
		lcd_puts_p(ST_TEMP_SETPALIER_TXT);
		lcd_gotoxy(0,1);
		lcd_puts(msg);
		sei();
	}
	else if(input == KEY_DOWN && tempPalier>=50)
	{
		tempPalier-=5;
		char msg[16];
		sprintf(msg,"%d",tempPalier);
		cli();
		lcd_clrscr();
		lcd_gotoxy(0,0);
		lcd_puts_p(ST_TEMP_SETPALIER_TXT);
		lcd_gotoxy(0,1);
		lcd_puts(msg);
		sei();
	}
	
	unsigned char nextstate = OS_stateMachine(OS_CURRENT_STATE, input);
	if (nextstate!=OS_CURRENT_STATE) // Si on quitte l'état
		first_run=TRUE;
	return nextstate;
}

char st_temp_pic(char input)
{
	static BOOL first_run = TRUE;
	if(first_run==TRUE)
	{
		char msg[16];
		sprintf(msg,"%d",tempPic);
		cli();
		lcd_clrscr();
		lcd_gotoxy(0,0);
		lcd_puts_p(ST_TEMP_SETPIC_TXT);
		lcd_gotoxy(0,1);
		lcd_puts(msg);
		sei();
		first_run=FALSE;
	}
	else if(input==KEY_UP && tempPic<=350)
	{
		tempPic+=5;
		char msg[16];
		sprintf(msg,"%d",tempPic);
		cli();
		lcd_clrscr();
		lcd_gotoxy(0,0);
		lcd_puts_p(ST_TEMP_SETPIC_TXT);
		lcd_gotoxy(0,1);
		lcd_puts(msg);
		sei();
	}
	else if(input == KEY_DOWN && tempPic>=150)
	{
		tempPic-=5;
		char msg[16];
		sprintf(msg,"%d",tempPic);
		cli();
		lcd_clrscr();
		lcd_gotoxy(0,0);
		lcd_puts_p(ST_TEMP_SETPIC_TXT);
		lcd_gotoxy(0,1);
		lcd_puts(msg);
		sei();
	}
	
	unsigned char nextstate = OS_stateMachine(OS_CURRENT_STATE, input);
	if (nextstate!=OS_CURRENT_STATE) // Si on quitte l'état
		first_run=TRUE;
	return nextstate;
}

char st_profiles_save(char input)
{
	static BOOL first_run = TRUE;
	
	
	
	unsigned char nextstate = OS_stateMachine(OS_CURRENT_STATE, input);
	if (nextstate!=OS_CURRENT_STATE) // Si on quitte l'état
		first_run=TRUE;
	return nextstate;
}

char st_profiles_load(char input)
{
	static BOOL first_run = TRUE;
	
	
	
	unsigned char nextstate = OS_stateMachine(OS_CURRENT_STATE, input);
	if (nextstate!=OS_CURRENT_STATE) // Si on quitte l'état
		first_run=TRUE;
	return nextstate;
}

char st_run(char input)
{
	static BOOL first_run = TRUE;
	static BOOL cmd_enabled = FALSE;
	
	if((cmd_enabled==FALSE)&&(input==KEY_ENTER))
	{
		PID_setParams(0.42,600,100);
		PID_start(150,5000);
		cmd_enabled=TRUE;
	}
	else if((cmd_enabled==TRUE)&&(input==KEY_ENTER))
	{
		PID_stop();
		cmd_enabled=FALSE;
	}
	
	
	unsigned char nextstate = OS_stateMachine(OS_CURRENT_STATE, input);
	if (nextstate!=OS_CURRENT_STATE) // Si on quitte l'état
		first_run=TRUE;
	return nextstate;
}

// manual menu
char st_manual(char input)
{
	static BOOL first_run = TRUE;
	
	
	
	unsigned char nextstate = OS_stateMachine(OS_CURRENT_STATE, input);
	if (nextstate!=OS_CURRENT_STATE) // Si on quitte l'état
		first_run=TRUE;
	return nextstate;
}

char st_manual_temp(char input)
{
	static BOOL first_run = TRUE;
	
	
	
	unsigned char nextstate = OS_stateMachine(OS_CURRENT_STATE, input);
	if (nextstate!=OS_CURRENT_STATE) // Si on quitte l'état
		first_run=TRUE;
	return nextstate;
}

char st_manual_cmd(char input)
{
	static BOOL first_run = TRUE;
	static BOOL cmd_enabled = FALSE;
	
	if(input==KEY_ENTER)
	{
		if(cmd_enabled)
		{
			PWM_disable();
			cli();
			lcd_clrscr();
			lcd_gotoxy(0,0);
			lcd_puts_p(ST_MANUAL_CMD_TXT);
			lcd_gotoxy(0,1);
			lcd_puts_P("0FF");
			CBID_MAXroutine = OS_removeCallback(dataLogger);
			sei();
		}
		else
		{
			PWM_enable();
			cli();
			lcd_clrscr();
			lcd_gotoxy(0,0);
			lcd_puts_p(ST_MANUAL_CMD_TXT);
			lcd_gotoxy(0,1);
			lcd_puts_P("0N");
			CBID_MAXroutine = OS_addCallback(dataLogger,5000);
			sei();
		}			
		cmd_enabled = ~cmd_enabled;
	}
	
	unsigned char nextstate = OS_stateMachine(OS_CURRENT_STATE, input);
	if (nextstate!=OS_CURRENT_STATE) // Si on quitte l'état
		first_run=TRUE;
	return nextstate;
}

char st_manual_set_cmd(char input)
{
	static BOOL first_run = TRUE;
	if(first_run==TRUE)
	{
		char msg[16];
		sprintf(msg,"%d",PWM_DC);
		cli();
		lcd_clrscr();
		lcd_gotoxy(0,0);
		lcd_puts_p(ST_MANUAL_SET_CMD_TXT);
		lcd_gotoxy(0,1);
		lcd_puts(msg);
		sei();
		first_run=FALSE;
	}
	else if(input==KEY_UP && PWM_DC<=250)
	{
		PWM_DC+=5;
		PWM_setDutyCycle(PWM_DC);
		char msg[16];
		sprintf(msg,"%d",PWM_DC);
		cli();
		lcd_clrscr();
		lcd_gotoxy(0,0);
		lcd_puts_p(ST_MANUAL_SET_CMD_TXT);
		lcd_gotoxy(0,1);
		lcd_puts(msg);
		sei();
	}
	else if(input == KEY_DOWN && PWM_DC>=5)
	{
		PWM_DC-=5;
		PWM_setDutyCycle(PWM_DC);
		char msg[16];
		sprintf(msg,"%d",PWM_DC);
		cli();
		lcd_clrscr();
		lcd_gotoxy(0,0);
		lcd_puts_p(ST_MANUAL_SET_CMD_TXT);
		lcd_gotoxy(0,1);
		lcd_puts(msg);
		sei();
	}
	
	unsigned char nextstate = OS_stateMachine(OS_CURRENT_STATE, input);
	if (nextstate!=OS_CURRENT_STATE) // Si on quitte l'état
		first_run=TRUE;
	return nextstate;
}

void dataLogger(void)
{
	char msg[64];
	sprintf(msg,"%d",MAX_getTemp());
	RS232_println(msg);
}
