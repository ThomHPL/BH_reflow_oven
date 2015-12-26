/*
 * I2C.c
 *
 * Created: 09/12/2015 07:49:44
 *  Author: BOSTEM Antoine & HERPOEL Thomas
 */ 

/*
 *	Pour l'utiliser, il faut creer une fonction et passer son pointeur à I2C_setFunction.
 *	Cette fonction devra prendre en argument le status code retourné par l'i2c.
 *	Elle sera alors éxecutée à chaque interruption dû à l'i2c et peut ainsi gérer les différents cas en fonctions des différents status code
 */

#include "main.h"
#include "I2C.h"
#include "RS232.h"

unsigned char SLA;
unsigned char I2C_statusCode;
char I2C_isSending;

// pointer to the function to be used 
void (*I2C_function)(unsigned char statusCode);

void I2C_init(int sclKHz)
{
	I2C_isSending = 0;
	TWCR |= (1<<TWEA)|(1<<TWEN)|(1<<TWIE);
	TWSR |= ((1<<TWPS1)|(0<<TWPS0));
	TWSR &= ~((0<<TWPS1)|(1<<TWPS0));
	TWBR = 4;	
}

void I2C_enableACK(void)
{
	TWCR |= (1<<TWEA);
}

void I2C_disableACK(void)
{
	TWCR &= ~(1<<TWEA);
}

void I2C_setAddress(unsigned char addr)
{
	SLA = addr;
}

void I2C_setFunction(void func(unsigned char))
{
	I2C_function=func;
}

void I2C_start(void)
{
	TWCR |= (1<<TWSTA);
	TWCR |= (1<<TWINT);
}

void I2C_stop(void)
{
	TWCR &= ~(1<<TWSTA);
	TWCR |= (1<<TWINT)|(1<<TWSTO);
}	

void I2C_send(unsigned char data)
{
	TWDR = data;
	TWCR &= ~((1<<TWSTA)|(1<<TWSTO));
	TWCR |= (1<<TWINT);
}

unsigned char I2C_receive(void)
{
	unsigned char data = TWDR;
	TWCR &= ~((1<<TWSTA)|(1<<TWSTO));
	TWCR |= (1<<TWINT);
	return data;
}

void I2C_sendSLAW()
{
	TWDR = (SLA<<1)|0b0;
	TWCR &= ~((1<<TWSTA)|(1<<TWSTO));
	TWCR |= (1<<TWINT);
}

void I2C_sendSLAR()
{
	TWDR = (SLA<<1)|0b1;
	TWCR &= ~((1<<TWSTA)|(1<<TWSTO));
	TWCR |= (1<<TWINT);
}

ISR(TWI_vect)
{
	I2C_statusCode = TWSR & 0b11111000;
	I2C_function(I2C_statusCode);
}