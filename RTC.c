/*
 * RTC.c
 *
 * Created: 15-12-15 12:47:37
 *  Author: Antoine
 */ 

#include "RTC.h"
#include "I2C.h"
unsigned char* buffer;
unsigned char pointer = 0;
unsigned char pageWidth=0;

unsigned char* RTC_read(unsigned char deviceAddr,unsigned int nBytes, unsigned char byteAddr)
{	
	while(I2C_isSending==1)
	{
		RS232_println_debug("I2C bus busy . . .");
	}
	
	I2C_isSending = 1;
	free(buffer);
	buffer = malloc(nBytes);
	pointer = byteAddr;
	pageWidth = nBytes;
	I2C_setAddress(deviceAddr);
	I2C_setFunction(read);
	I2C_start();
	while(I2C_isSending==1)
	{
		statusLedToggle();
	}
	return buffer;
}

void read(unsigned char statusCode)
{
	static int index = 0;
	
	switch (statusCode)
	{
	case M_START_ACK :
		I2C_sendSLAW();
		break;
	case M_SLAW_NACK :
		I2C_stop();
		index=0;
		I2C_start();
		return;
	case M_SLAW_ACK :
		I2C_send(pointer);
		break;
	case M_DATA_TX_ACK :
		I2C_start();
		break;
	case M_REP_START_ACK:
		I2C_sendSLAR();
		break;
	case M_SLAR_ACK :
		if(index == (pageWidth+3))
			I2C_disableACK();
		*buffer=I2C_receive();
		buffer++;
		pointer++;
		break;
	case M_DATA_RX_ACK :
		if(index==(pageWidth+3))
			I2C_disableACK();
		*buffer=I2C_receive();
		buffer++;
		pointer++;	
		break;
	case M_DATA_RX_NACK :
		I2C_stop();
		index = 0;
		I2C_enableACK();
		I2C_isSending = 0;
		return;
	}
	index+=1;
}

//
//#include "RTC.h"
//#include "I2C.h"
//
//// Fonction pour définir l'adresse de la RTC
//
//void RTC_setStartAdress(void)
//{
	//I2C_start();
	//I2C_sendSLAW(DS1307_W);
	//I2C_send(0x00);
	//I2C_stop();
//}
//
//
//unsigned char RTC_read(void)
//{unsigned char i,data;
///*unsigned char errorStatus,i,data;
	//errorStatus = I2C_start();
		//if(errorStatus==1)			//for debug
		//{
			//lcd_clrscr();
			//int i=0;
			//do 
			//{
				//lcd_gotoxy(0,0);lcd_puts(" Erreur I2C");
				//lcd_gotoxy(0,1);lcd_puts(" Start");
				//i++;
			//} while (i!=125);
		//return ST_WELCOME_ID;
		//}*/
	//// errorStatus = I2C_sendSLAR()
	//
 //
    //for(i=0;i<7;i++)
   //{
      //if(i == 6)  	 //no Acknowledge after receiving the last byte
	   	  //data = I2C_receiveData_NACK();
	  //else
	  	  //data = I2C_receiveData_ACK();
			//
	  //rtc_register[i] = data;
	//}
	//I2C_stop();
	//return ST_WELCOME_ID;
		//
		//
//}
//
//void RTC_getTime(void)
//{
   //RTC_read();
   //time[7] = (SECONDS & 0x0f) | 0x30;		//seconds(1's)
   //time[6] = ((SECONDS & 0x70) >> 4) | 0x30;		//seconds(10's)
   //time[5] = ':';
   //time[4] = (MINUTES & 0x0f) | 0x30;
   //time[3] = ((MINUTES & 0x70) >> 4) | 0x30;
   //time[2] = ':'; 
   //time[1] = (HOURS & 0x0f) | 0x30;	
   //time[0] = ((HOURS & 0x30) >> 4) | 0x30;
//}
//
//void RTC_getDate(void)
//{
  //RTC_read();
  ////date[11] = 0x00;
  ////date[10] = 0x00;
  //date[9] = (YEAR & 0x0f) | 0x30;
  //date[8] = ((YEAR & 0xf0) >> 4) | 0x30;
  //date[7] = '0';
  //date[6] = '2';
  //date[5] = '/';
  //date[4] = (MONTH & 0x0f) | 0x30;
  //date[3] = ((MONTH & 0x10) >> 4) | 0x30;
  //date[2] = '/';
  //date[1] = (DATE & 0x0f) | 0x30;
  //date[0] = ((DATE & 0x30) >> 4) | 0x30;
//}  
//
//char RTC_displayTime(char input)
//{
	//lcd_clrscr();
	//RTC_getDate();
	//RTC_getTime();   
  	//int i;
	//i=0;
			//do
			//{
			//lcd_gotoxy(0,0); lcd_puts(date);
			//lcd_gotoxy(0,1); lcd_puts(time);
			//i++;
			//} while (i!=100); //transmitString(time);  
//return ST_WELCOME_ID;
//}
//
//void RTC_updateRegisters(void)
//{
  //SECONDS = ((time[6] & 0x07) << 4) | (time[7] & 0x0f);
  //MINUTES = ((time[3] & 0x07) << 4) | (time[4] & 0x0f);
  //HOURS = ((time[0] & 0x03) << 4) | (time[1] & 0x0f);  
  //DAY = date[10];
  //DATE = ((date[0] & 0x03) << 4) | (date[1] & 0x0f);
  //MONTH = ((date[3] & 0x01) << 4) | (date[4] & 0x0f);
  //YEAR = ((date[8] & 0x0f) << 4) | (date[9] & 0x0f);
//}  
//
//unsigned char RTC_writeTime(void)
//{
	//unsigned char errorStatus, i;
  ///* errorStatus = I2C_start();
   //if(errorStatus == 1)
   //{lcd_clrscr();
			//int i=0;
  //do
			//{
			//lcd_gotoxy(0,0); lcd_puts("   Erreur I2C   ");
			//lcd_gotoxy(0,1); lcd_puts("      Start     ");
			//i++;
			//} while (i!=125);
	//return ST_WELCOME_ID;
   //} */
   //for(i=0;i<3;i++)
    //{
	  //I2C_send(rtc_register[i]);  
	//
   	  //}
   //
	//I2C_stop();
	//return(0);
   //
//}
//unsigned char RTC_writeDate(void)
//{
 //unsigned char errorStatus, i;
  ///* errorStatus = I2C_start();
   //if(errorStatus == 1)
   //{lcd_clrscr();
			//int i=0;
  //do
			//{
			//lcd_gotoxy(0,0); lcd_puts("   Erreur I2C   ");
			//lcd_gotoxy(0,1); lcd_puts("      Start     ");
			//i++;
			//} while (i!=125);
	//return ST_WELCOME_ID;
   //} */
   //for(i=3;i<7;i++)
    //{
	   //I2C_send(rtc_register[i]);  
   	 //
   	  //}
    //
	//I2C_stop();
	//return ST_WELCOME_ID;
   //
//}   
//
//char RTC_updateTime(void)
//{
	//unsigned char data;
   	//time[0]= '0';
	//time[1]= '9';
	//time[3]= '3';
	//time[4]= '0'; 
	//time[6]= '0'; 
	//time[7]= '0'; 
  //RTC_updateRegisters(); 
  //data = RTC_writeTime();
  //RTC_updateDate();
  //return ST_WELCOME_ID;
//}
//void RTC_updateDate(void)
//{
  //unsigned char data;	   	  	  				
	//date[0]= '0';
	//date[1]= '2';
	//date[2] = '/';
	//date[3]= '0'; 
	//date[4] = '1';
	//date[5] = '/';
	//date[6] = '2';
	//date[7] = '0';   
	//date[8]= '1';
	//date[9] = '5';//& 0x0f;
  //RTC_updateRegisters(); 
  //data = RTC_writeDate();
  //return;
//}   