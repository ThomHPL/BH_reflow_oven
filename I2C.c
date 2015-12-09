/*
 * I2C.c
 *
 * Created: 09/12/2015 07:49:44
 *  Author: Thomas
 */ 

#include "main.h"
#include "I2C.h"
#include "USART.h"

unsigned char SLA;
unsigned char SPI_statusCode;

// pointer to the function to be used 
void (*I2C_function)(unsigned char statusCode);

void I2C_init(int sclKHz)
{
	TWBR = ((CPU_F/1000)-(sclKHz))/(2*sclKHz*PRESCALER);
	RS232_sendByte(TWBR);
	RS232_sendByte(0x55);
	TWCR |= (1<<TWEA)|(1<<TWEN)|(1<<TWIE);
	TWSR &= ~((1<<TWPS1)|(1<<TWPS0));
}

void I2C_setAddress(unsigned char addr)
{
	SLA = addr;
}
void I2C_setFunction(void *func(unsigned char))
{
	I2C_function=func;
}	

void I2C_start(void)
{
	TWCR |= (1<<TWINT)|(1<<TWSTA);
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
	SPI_statusCode = TWCR;
	I2C_function(SPI_statusCode);
}