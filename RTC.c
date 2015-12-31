/*
 * RTC.c
 *
 * Created: 15-12-15 12:47:37
 *  Author: BOSTEM Antoine & HERPOEL Thomas
 */ 

#include "main.h"
#include "RTC.h"
#include "I2C.h"
#include "hardware.h"
#include "RS232.h"
unsigned char* buffer;
unsigned char pointerRTC = 0;
unsigned char pageWidthRTC=0;
unsigned char DATA[8];

void RTC_getTime(char* str)
{
	// "HH:MM:SS"+0x00
	// voir datasheet pour savoir la position des differentes info
	DATA[0]=*RTC_read(DS1307_ADDR,1,0);
	DATA[1]=*RTC_read(DS1307_ADDR,1,1);
	DATA[2]=*RTC_read(DS1307_ADDR,1,2);
	str[8]=0x00;
	str[7]=(DATA[0]&0b1111) | 0x30;
	str[6]=((DATA[0]&0b1110000)>>4) | 0x30;
	str[5]=':';
	str[4]=(DATA[1]&0b1111) | 0x30;
	str[3]=((DATA[1]&0b1110000)>>4) | 0x30;
	str[2]=':';
	str[1]=(DATA[2]&0b1111) | 0x30;
	str[0]=((DATA[2]&0b110000)>>4) | 0x30;
}

/************************************************************************/
/* TESTED OK                                                            */
/************************************************************************/
unsigned char* RTC_read(unsigned char deviceAddr,unsigned int nBytes, unsigned char byteAddr)
{	
	while(I2C_isSending==1)
	{
		RS232_println_debug("I2C bus busy . . .");
	}
	
	I2C_isSending = 1;
	free(buffer);
	buffer = malloc(nBytes);
	pointerRTC = byteAddr;
	pageWidthRTC = nBytes;
	I2C_setAddress(deviceAddr);
	I2C_setFunction(read);
	I2C_start();
	while(I2C_isSending==1)
	{
		statusLedToggle();
	}
	return buffer;
}

/************************************************************************/
/* TESTED OK                                                            */
/************************************************************************/
void read(unsigned char statusCode)
{
	static int index = 0;
	static unsigned char n = 0;
	
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
		I2C_send(pointerRTC);
		break;
	case M_DATA_TX_ACK :
		I2C_start();
		break;
	case M_REP_START_ACK:
		I2C_sendSLAR();
		break;
	case M_SLAR_ACK :
		if(index == (pageWidthRTC+3))
			I2C_disableACK();
		I2C_receive();
		pointerRTC++;
		break;
	case M_DATA_RX_ACK :
		if(index==(pageWidthRTC+3))
			I2C_disableACK();
		buffer[n]=I2C_receive();
		n++;
		pointerRTC++;
		break;
	case M_DATA_RX_NACK :
		I2C_stop();
		buffer[n]=I2C_receive();
		n=0;
		index = 0;
		I2C_enableACK();
		I2C_isSending = 0;
		return;
	}
	index+=1;
}
