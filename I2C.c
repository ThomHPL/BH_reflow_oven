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
unsigned char I2C_statusCode;

// pointer to the function to be used 
void (*I2C_function)(unsigned char statusCode);

void I2C_init(int sclKHz)
{
	TWCR |= (1<<TWEA)|(1<<TWEN)|(1<<TWIE);
	TWSR |= ((1<<TWPS1)|(0<<TWPS0));
	TWSR &= ~((0<<TWPS1)|(1<<TWPS0));
	TWBR = 4;	
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
	
	/*
	RS232_print("TWI interruption: ");
	char msg[16];
	sprintf(msg,"0x%X",I2C_statusCode);
	RS232_println(msg);
	*/	
}