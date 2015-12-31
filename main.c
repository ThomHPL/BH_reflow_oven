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
char CBID_MAXroutine = 0;
char CBID_DataLogger = 0;

// running parameters
int blinkPeriod=1000;
unsigned char PWM_DC = 127;
int	TEMP = 50;
int	tempPalier = 150;
int tempPic = 250;

// MAIN
int main(void)
{	
	// initialisation du systeme
	lcd_init(LCD_DISP_ON);
	RS232_init(9600);	
	statusLed_init();
	KEYBOARD_init();
	MAX_init();
	I2C_init(100);
	PWM_Init(PWM_DC);
	OS_init();
	sei();
	
	// paramètres du régulateur mixte
	// PID_setParams(0.84,250,100);
	PID_setParams(1.2,250.0,10.0);

	// démarrage des callback	
	CBID_TglLed	= OS_addCallback(statusLedToggle,blinkPeriod);
	CBID_MAXroutine = OS_addCallback(MAX_start,20);					// launch a measure every 20 ms
	
	// header
	RS232_println("BH REFLOW OVEN V01.00.00");
	RS232_println("");
	char str[9];
	RTC_getTime(str);
	RS232_println(str);
	
	// démarrage de l'OS
	OS_start();
		
	return 1;
}

// main menu
// Tested OK
// affiche l'écran initial
char st_welcome(char input)
{
	static int32_t oldTmp=0;
	int32_t tmp = 0;
	tmp = MAX_getTemp();
	
	if(tmp!=oldTmp||OS_first_run==TRUE)
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

	}
	
	return OS_getNextState(OS_CURRENT_STATE, input);
}
// Tested OK
// affiche l'écran servant à modifier la température de palier
char st_temp_palier(char input)
{
	if(OS_first_run==TRUE)
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
	
	return OS_getNextState(OS_CURRENT_STATE, input);
}
// Tested OK
// affiche l'écran servant à modifier la température de pic
char st_temp_pic(char input)
{
	if(OS_first_run==TRUE)
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
	
	return OS_getNextState(OS_CURRENT_STATE, input);
}
// Tested OK
// affiche l'écran servant à sauver le profil dans l'eeprom
char st_profiles_save(char input)
{
	if(input==KEY_ENTER)
	{
		unsigned char* data=malloc(4);
		data[0]=(unsigned char)(tempPalier>>8)&0xFF;
		data[1]=(unsigned char)(tempPalier&0xFF);
		data[2]=(unsigned char)(tempPic>>8)&0xFF;
		data[3]=(unsigned char)(tempPic&0xFF);
		EEPROM_writePage(data,EEPROM_ADDR,0,16);
	}
	
	return OS_getNextState(OS_CURRENT_STATE, input);
}
// Tested OK
// affiche l'écran servant à charger le profil stocké dans l'eeprom
char st_profiles_load(char input)
{
	if(input==KEY_ENTER)
	{
		unsigned char* data=EEPROM_readPage(EEPROM_ADDR,0,16);
		tempPalier=(int)((data[0]<<8)|data[1]);
		tempPic=(int)((data[2]<<8)|data[3]);
	}
	
	return OS_getNextState(OS_CURRENT_STATE, input);
}

// affiche l'écran servant à lancer la régulation du four selon le profil
char st_run(char input)
{
	static BOOL cmd_enabled = FALSE;
	
	if((cmd_enabled == FALSE)&&(input == KEY_ENTER))
	{
		PID_setConsigne(tempPalier);
		PID_start();
		//CBID_DataLogger=OS_addCallback(dataLogger,1000);
		cmd_enabled=TRUE;
	}
	else if((cmd_enabled == TRUE)&&(input == KEY_ENTER))
	{
		PID_stop();
		//OS_removeCallback(CBID_DataLogger);
		cmd_enabled=FALSE;
	}
	
	return OS_getNextState(OS_CURRENT_STATE, input);
}

// manual menu
// Tested OK
// affiche l'écran servant à lancer une régulation à une tempérture entrée
char st_manual_temp(char input)
{
	static BOOL temp_enabled = FALSE;
	
	if(OS_first_run==TRUE)
	{
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
	
	return OS_getNextState(OS_CURRENT_STATE, input);
}
// Tested OK
// affiche l'écran servant à modifier la température pour la régulation manuelle
char st_manual_set_temp(char input)
{
	if(OS_first_run==TRUE)
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
	
	return OS_getNextState(OS_CURRENT_STATE, input);
}
// Tested OK
// affiche l'écran servant à effectuer un commande (un step)
char st_manual_cmd(char input)
{
	static BOOL cmd_enabled = FALSE;
	
	if(OS_first_run==TRUE)
	{
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
	
	return OS_getNextState(OS_CURRENT_STATE, input);
}
// Tested OK
// affiche l'écran servant à modifier la commande pour le step
char st_manual_set_cmd(char input)
{
	if(OS_first_run==TRUE)
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
	
	return OS_getNextState(OS_CURRENT_STATE, input);
}

void dataLogger(void)
{
	char msg[64];
	sprintf(msg,"%d",MAX_getTemp());
	RS232_println(msg);
}
