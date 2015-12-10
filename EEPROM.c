/*
 * EEPROM.c
 *
 * Created: 09/12/2015 18:13:00
 *  Author: Thomas
 */ 
#include "EEPROM.h"
#include "I2C.h"
unsigned int pointer;
unsigned int pageWidth;
unsigned char* buffer;

// internal functions
void writeByte(unsigned char statusCode);
void writePage(unsigned char statusCode);

void EEPROM_writeByte(unsigned char data,unsigned char deviceAddr,unsigned int byteAddr)
{
	// TODO: test if the address is at the beginning of a page
	pointer = byteAddr;
	buffer = &data;
	I2C_setAddress(deviceAddr);
	I2C_setFunction(writeByte);
	I2C_start();
}

void EEPROM_writePage(unsigned char* data,unsigned char deviceAddr,unsigned int pageAddr,unsigned char pageSize)
{
	// TODO: test if the address is at the beginning of a page
	pointer = pageAddr;
	pageWidth = pageSize;
	buffer = data;
	I2C_setAddress(deviceAddr);
	I2C_setFunction(writePage);
	I2C_start();
}

/************************************************************************/
/* Function to write a byte												*/
/* Will be executed at each I2C interrupt                               */
/************************************************************************/
void writeByte(unsigned char statusCode)
{
	static int index = 0;
	
	switch (statusCode)
	{
	case M_START_ACK : 
		I2C_sendSLAW();
		break;
	case M_SLAW_ACK :
		I2C_send((unsigned char)(pointer>>8));
		break;
	case M_DATA_TX_ACK :
		if(index==2)
			I2C_send((unsigned char)(pointer&0xFF));
		else if (index == 3)
		{
			I2C_send(*buffer);
			pointer +=1;
		}			
		else
		{
			I2C_stop();
			index = 0;
			return ;
		}
		break;
	}
	
	index+=1;
}

/************************************************************************/
/* Function to write a page												*/
/* Will be executed at each I2C interrupt                               */
/************************************************************************/
void writePage(unsigned char statusCode)
{
	static int index = 0;
	
	switch (statusCode)
	{
	case M_START_ACK : 
		I2C_sendSLAW();
		break;
	case M_SLAW_ACK :
		I2C_send((unsigned char)(pointer>>8));
		break;
	case M_DATA_TX_ACK :
		if(index==2)
			I2C_send((unsigned char)(pointer&0xFF));
		else if (index < 35)
		{
			I2C_send(*buffer++);
			pointer +=1;
		}			
		else
		{
			I2C_stop();
			index = 0;
			return ;
		}
		break;
	}
	
	index+=1;
}