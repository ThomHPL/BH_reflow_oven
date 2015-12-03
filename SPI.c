/*
 * SPI.c
 *
 * Created: 16/11/2015 17:59:46
 *  Author: Thomas
 */ 

// Global variable

#include "main.h"
#include "MAX.h"
#include "SPI.h"
#include "USART.h"

uint32_t frame = NULL;
char nbits = 32;

//unsigned int frame = 0;
unsigned char count = 0;
int temp = 0;

void SPI_init(unsigned char nbit)
{
	// configured as a master (MSTR in SPCR)
	// SS pin configured as output
	nbits = nbit;
	
	sbiBF(SPI_DDR,SPI_SS);
	sbiBF(SPI_PORT,SPI_SS);
	
	sbiBF(SPI_DDR,SPI_SCK);
	cbiBF(SPI_PORT,SPI_SCK);
	
	cbiBF(SPI_DDR,SPI_MISO);
	
	SPCR = (1<<SPIE)|(0<<SPE)|(1<<MSTR)|(SPI_CPOL<<CPOL)|(SPI_CPHA<<CPHA)|(1<<SPR1);
	
	count = (nbits/8)-1;
	frame = 0;	
}

void SPI_start()
{
	frame = 0;
	sbiBF(SPCR,SPE);
	cbiBF(SPI_PORT,SPI_SS);
	SPDR = 0x55;
}

uint32_t SPI_getFrame()
{
	return frame;
}

ISR(SPI_STC_vect)
{	
	frame = (frame<<8) | SPDR;
	if(count==0)
	{
		sbiBF(SPI_PORT,SPI_SS);
		cbiBF(SPCR,SPE);
		count = (nbits/8)-1;
		char msg[16];
		sprintf(msg,",%d",MAX_getTemp());
		RS232_print(&msg);
	}
	else
		count-=1;
	SPDR = 0x55;
}