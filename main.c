/*
 * main.c
 *
 * Created: 30/10/2015 14:29:45
 *  Author: BOSTEM Antoine & HERPOEL Thomas
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

void delay(int ms);
void delaycb(void);
void init(void);
void dataLogger(void);

char CBID_TglLed = 0;
int blinkPeriod=1000;
char CBID_MAXroutine = 0;
char CBID_DataLogger = 0;

unsigned char PWM_DC = 127;
int	TEMP = 50;
int	tempPalier = 150;
int tempPic = 250;

// MAIN
int main(void)
{	
	lcd_init(LCD_DISP_ON);
	RS232_init(9600);	
	statusLed_init();
	KEYBOARD_init();
	MAX_init();
	I2C_init(100);
	
	OS_init();
	
	sei();
	
	// header
	RS232_println("BH REFLOW OVEN V01.00.00");
	RS232_println("");
	char str[9];
	RTC_getTime(str);
	RS232_println(str);
	
	PWM_Init(PWM_DC);
	
	CBID_TglLed	= OS_addCallback(statusLedToggle,blinkPeriod);
	CBID_MAXroutine = OS_addCallback(MAX_start,20);					// launch a measure every 20 ms

	PID_setParams(0.42,600,100);
			
	OS_start();
		
	return 1;
}

// main menu
// Tested OK
char st_welcome(char input)
{
	static BOOL first_run = TRUE;
	static int32_t oldTmp=0;
	int32_t tmp = 0;
	tmp = MAX_getTemp();
	
	if(tmp!=oldTmp||first_run==TRUE)
	{
		char msg[16];
		
		cli();
		lcd_clrscr();
		lcd_gotoxy(0,0);
		lcd_puts_p(ST_WELCOME_TXT);
		lcd_gotoxy(0,1);
		sprintf(msg,"%d|%d 'C",(int)tmp,tempPalier);
		lcd_puts(msg);
		sei();
		oldTmp=tmp;
		first_run=FALSE;
	}
	
	unsigned char nextstate = OS_stateMachine(OS_CURRENT_STATE, input);
	if (nextstate!=OS_CURRENT_STATE) // Si on quitte l'état
		first_run=TRUE;
	return nextstate;
}
// Tested OK
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
// Tested OK
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
// Tested OK
char st_profiles_save(char input)
{
	static BOOL first_run = TRUE;
	
	if(input==KEY_ENTER)
	{
		unsigned char* data=malloc(4);
		data[0]=(unsigned char)(tempPalier>>8)&0xFF;
		data[1]=(unsigned char)(tempPalier&0xFF);
		data[2]=(unsigned char)(tempPic>>8)&0xFF;
		data[3]=(unsigned char)(tempPic&0xFF);
		EEPROM_writePage(data,EEPROM_ADDR,0,16);
	}
	
	unsigned char nextstate = OS_stateMachine(OS_CURRENT_STATE, input);
	if (nextstate!=OS_CURRENT_STATE) // Si on quitte l'état
		first_run=TRUE;
	return nextstate;
}
// Tested OK
char st_profiles_load(char input)
{
	static BOOL first_run = TRUE;
	
	if(input==KEY_ENTER)
	{
		unsigned char* data=EEPROM_readPage(EEPROM_ADDR,0,16);
		tempPalier=(int)((data[0]<<8)|data[1]);
		tempPic=(int)((data[2]<<8)|data[3]);
	}
	
	unsigned char nextstate = OS_stateMachine(OS_CURRENT_STATE, input);
	if (nextstate!=OS_CURRENT_STATE) // Si on quitte l'état
		first_run=TRUE;
	return nextstate;
}

char st_run(char input)
{
	static BOOL first_run = TRUE;
	static BOOL cmd_enabled = FALSE;
	
	if((cmd_enabled == FALSE)&&(input == KEY_ENTER))
	{
		PID_setConsigne(tempPalier);
		PID_start();
		cmd_enabled=TRUE;
	}
	else if((cmd_enabled == TRUE)&&(input == KEY_ENTER))
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
// Tested OK
char st_manual(char input)
{
	static BOOL first_run = TRUE;
	
	
	
	unsigned char nextstate = OS_stateMachine(OS_CURRENT_STATE, input);
	if (nextstate!=OS_CURRENT_STATE) // Si on quitte l'état
		first_run=TRUE;
	return nextstate;
}
// Tested OK
char st_manual_temp(char input)
{
	static BOOL first_run = TRUE;
	static BOOL temp_enabled = FALSE;
	
	if(first_run==TRUE)
	{
		first_run=FALSE;
		if(temp_enabled==TRUE)
		{
			cli();
			lcd_clrscr();
			lcd_gotoxy(0,0);
			lcd_puts_p(ST_MANUAL_TEMP_TXT);
			lcd_gotoxy(0,1);
			lcd_puts_P("0N");
			sei();
		}
		else
		{
			cli();
			lcd_clrscr();
			lcd_gotoxy(0,0);
			lcd_puts_p(ST_MANUAL_TEMP_TXT);
			lcd_gotoxy(0,1);
			lcd_puts_P("OFF");
			sei();
		}
		
	}
	else if(input == KEY_ENTER)
	{
		if(temp_enabled == TRUE)
		{
			cli();
			lcd_clrscr();
			lcd_gotoxy(0,0);
			lcd_puts_p(ST_MANUAL_TEMP_TXT);
			lcd_gotoxy(0,1);
			lcd_puts_P("0FF");
			temp_enabled=FALSE;
			PID_stop();
			sei();
		}
		else
		{
			cli();
			lcd_clrscr();
			lcd_gotoxy(0,0);
			lcd_puts_p(ST_MANUAL_TEMP_TXT);
			lcd_gotoxy(0,1);
			lcd_puts_P("0N");
			temp_enabled=TRUE;
			PID_start();
			sei();
		}
	}
	
	
	unsigned char nextstate = OS_stateMachine(OS_CURRENT_STATE, input);
	if (nextstate!=OS_CURRENT_STATE) // Si on quitte l'état
		first_run=TRUE;
	return nextstate;
}
// Tested OK
char st_manual_set_temp(char input)
{
	static BOOL first_run = TRUE;
	if(first_run==TRUE)
	{
		char msg[16];
		sprintf(msg,"%d",TEMP);
		cli();
		lcd_clrscr();
		lcd_gotoxy(0,0);
		lcd_puts_p(ST_MANUAL_SET_TEMP_TXT);
		lcd_gotoxy(0,1);
		lcd_puts(msg);
		sei();
		first_run=FALSE;
	}
	else if(input==KEY_UP && TEMP<=300)
	{
		TEMP+=5;
		char msg[16];
		sprintf(msg,"%d",TEMP);
		cli();
		lcd_clrscr();
		lcd_gotoxy(0,0);
		lcd_puts_p(ST_MANUAL_SET_TEMP_TXT);
		lcd_gotoxy(0,1);
		lcd_puts(msg);
		PID_setConsigne(TEMP);
		sei();
	}
	else if(input == KEY_DOWN && TEMP>=25)
	{
		TEMP-=5;
		char msg[16];
		sprintf(msg,"%d",TEMP);
		cli();
		lcd_clrscr();
		lcd_gotoxy(0,0);
		lcd_puts_p(ST_MANUAL_SET_TEMP_TXT);
		lcd_gotoxy(0,1);
		lcd_puts(msg);
		PID_setConsigne(TEMP);
		sei();
	}
	
	unsigned char nextstate = OS_stateMachine(OS_CURRENT_STATE, input);
	if (nextstate!=OS_CURRENT_STATE) // Si on quitte l'état
		first_run=TRUE;
	return nextstate;
}
// Tested OK
char st_manual_cmd(char input)
{
	static BOOL first_run = TRUE;
	static BOOL cmd_enabled = FALSE;
	
	if(first_run==TRUE)
	{
		first_run=FALSE;
		if(cmd_enabled==TRUE)
		{
			cli();
			lcd_clrscr();
			lcd_gotoxy(0,0);
			lcd_puts_p(ST_MANUAL_CMD_TXT);
			lcd_gotoxy(0,1);
			lcd_puts_P("0N");
			sei();
		}
		else
		{
			cli();
			lcd_clrscr();
			lcd_gotoxy(0,0);
			lcd_puts_p(ST_MANUAL_CMD_TXT);
			lcd_gotoxy(0,1);
			lcd_puts_P("OFF");
			sei();
		}
		
	}
	else if(input==KEY_ENTER)
	{
		if(cmd_enabled==TRUE)
		{
			cli();
			lcd_clrscr();
			lcd_gotoxy(0,0);
			lcd_puts_p(ST_MANUAL_CMD_TXT);
			lcd_gotoxy(0,1);
			lcd_puts_P("0FF");
			//CBID_MAXroutine = OS_removeCallback(dataLogger);
			PWM_disable();
			cmd_enabled=FALSE;
			sei();
		}
		else
		{
			cli();
			lcd_clrscr();
			lcd_gotoxy(0,0);
			lcd_puts_p(ST_MANUAL_CMD_TXT);
			lcd_gotoxy(0,1);
			lcd_puts_P("0N");
			//CBID_MAXroutine = OS_addCallback(dataLogger,5000);
			cmd_enabled=TRUE;
			PWM_enable();
			sei();
		}
	}
	
	unsigned char nextstate = OS_stateMachine(OS_CURRENT_STATE, input);
	if (nextstate!=OS_CURRENT_STATE) // Si on quitte l'état
		first_run=TRUE;
	return nextstate;
}
// Tested OK
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
