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
void erasePage(unsigned char statusCode);

void EEPROM_writeByte(unsigned char byte,unsigned char deviceAddr,unsigned int byteAddr)
{
	// BUG !!
	pointer = byteAddr;
	pageWidth = 1;
	buffer = &byte;
	I2C_setAddress(deviceAddr);
	I2C_setFunction(writePage);
	I2C_start();
}

void EEPROM_writePage(unsigned char* data,unsigned char deviceAddr,unsigned int pageAddr,unsigned char pageSize)
{
	// test if the address is at the beginning of a page
	if((pageAddr&(pageSize-1))!=0)
		return;

	pointer = pageAddr;
	pageWidth = pageSize;
	buffer = data;
	I2C_setAddress(deviceAddr);
	I2C_setFunction(writePage);
	I2C_start();
}

void EEPROM_erasePage(unsigned char deviceAddr,unsigned int pageAddr,unsigned char pageSize)
{
	// test if the address is at the beginning of a page
	if((pageAddr&(pageSize-1))!=0)
		return;

	pointer = pageAddr;
	pageWidth = pageSize;
	I2C_setAddress(deviceAddr);
	I2C_setFunction(erasePage);
	I2C_start();
}

/************************************************************************/
/* Function to write a byte												*/
/* Will be executed at each I2C interrupt                               */
/* TESTED KO															*/
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
			return ;
		}
		break;
	}
	index+=1;
}

/************************************************************************/
/* Function to erase a page												*/
/* Will be executed at each I2C interrupt                               */
/* TESTED OK															*/
/************************************************************************/
void erasePage(unsigned char statusCode)
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
		else if (index < (pageWidth+3))
		{
			I2C_send(0xFF);
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