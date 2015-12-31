/*
 * EEPROM.c
 *
 * Created: 09/12/2015 18:13:00
 *  Author: BOSTEM Antoine & HERPOEL Thomas
 */ 
#include "main.h"
#include "EEPROM.h"
#include "I2C.h"
#include "RS232.h"
#include "hardware.h"

unsigned int	pointer;
unsigned int	pageWidth;
unsigned char*	buffer;

// Fonctions internes
void readPage(unsigned char statusCode);
void writePage(unsigned char statusCode);
void erasePage(unsigned char statusCode);

unsigned char* EEPROM_readPage(unsigned char deviceAddr,unsigned int pageAddr, unsigned char pageSize)
{
	if((pageAddr&(pageSize-1))!=0)
		return buffer;
	
	while(I2C_isSending==1)
	{
		RS232_println_debug("EEPROM busy . . .");
	}
	
	I2C_isSending = 1;
	free(buffer);
	buffer = malloc(pageSize);
	pointer = pageAddr;
	pageWidth = pageSize;
	I2C_setAddress(deviceAddr);
	I2C_setFunction(readPage);
	I2C_start();
	while(I2C_isSending==1)	// attend la fin de la transmission
	{
		statusLedToggle();
		statusLedToggle();
	}
	return buffer;
}	

void EEPROM_writePage(unsigned char* data,unsigned char deviceAddr,unsigned int pageAddr,unsigned char pageSize)
{
	// test if the address is at the beginning of a page
	if((pageAddr&(pageSize-1))!=0)
		return;
	while(I2C_isSending==1)
	{
		RS232_println_debug("EEPROM busy . . .");
	}

	I2C_isSending = 1;	
	pointer = pageAddr;
	pageWidth = pageSize;
	free(buffer);
	buffer = data;
	I2C_setAddress(deviceAddr);
	I2C_setFunction(writePage);
	I2C_start();
}

void EEPROM_erasePage(unsigned char deviceAddr,unsigned int pageAddr,unsigned char pageSize)
{
	while(I2C_isSending==1)
	{
		RS232_println_debug("EEPROM busy . . .");
	}
	static unsigned char* page;
	free(page);
	page=malloc(pageSize);
	for(int i=0;i<pageSize;i++)
	{
		*(page+i)=0xFF;
	}
	EEPROM_writePage(page,deviceAddr,pageAddr,pageSize);
}

/************************************************************************/
/* Function to read a page												*/
/* Will be executed at each I2C interrupt                               */
/* TESTED OK															*/
/************************************************************************/
void readPage(unsigned char statusCode)
{
	static int index = 0;
	static unsigned char n=0;
	
	switch (statusCode)
	{
	case M_START_ACK :
		I2C_sendSLAW();
		break;
	case M_SLAW_NACK :		// means the eeprom is not responding
		I2C_stop();
		index=0;
		I2C_start();
		return;
	case M_SLAW_ACK :
		I2C_send((unsigned char)(pointer>>8));
		break;
	case M_DATA_TX_ACK :
		if(index == 2)
			I2C_send((unsigned char)(pointer&0xFF));
		else if(index == 3)
			I2C_start();
		break;
	case M_REP_START_ACK:
		I2C_sendSLAR();
		break;
	case M_SLAR_ACK :
		if (index == (pageWidth+4))
			I2C_disableACK();
		I2C_receive();
		pointer +=1;
		break;
	case M_DATA_RX_ACK :
		if (index == (pageWidth+4))
			I2C_disableACK();
		buffer[n]=I2C_receive();
		n++;
		pointer+=1;		
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

/************************************************************************/
/* Function to write a page												*/
/* Will be executed at each I2C interrupt                               */
/* TESTED OK															*/
/************************************************************************/
void writePage(unsigned char statusCode)
{
	static int index = 0;
	
	switch (statusCode)
	{
	case M_START_ACK : 
		I2C_sendSLAW();
		break;
	case M_SLAW_NACK :		// means the eeprom is not responding
		RS232_println("EEPROM BUSY!");
		I2C_stop();
		index=0;
		I2C_start();
		return;
	case M_SLAW_ACK :
		I2C_send((unsigned char)(pointer>>8));
		break;
	case M_DATA_TX_ACK :
		if(index==2)
			I2C_send((unsigned char)(pointer&0xFF));
		else if (index < (pageWidth+3))
		{
			I2C_send(*buffer++);
			pointer +=1;
		}			
		else
		{
			I2C_stop();
			index = 0;
			I2C_isSending=0;
			free(buffer);
			return ;
		}
		break;
	}
	index+=1;
}
