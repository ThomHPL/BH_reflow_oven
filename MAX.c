/*
 * MAX.c
 *
 * Created: 25/11/2015 14:04:52
 *  Author: BOSTEM Antoine & HERPOEL Thomas
 */ 

#include "main.h"
#include "MAX.h"
#include "SPI.h"

void MAX_init(void)
{
	SPI_init(FRAME_SIZE);
}
void MAX_start(void)
{
	SPI_start();
}
int MAX_getTemp(void)
{
	uint32_t tmp = SPI_getFrame();
	tmp = tmp >> TEMP_POS;
	
	int temp = (int)(tmp & ((1<<(TEMP_SIZE-1))-1));
	temp = temp/4;									//  LSB = 0.25°C
	
	if((tmp&(1<<(TEMP_SIZE-1)))==1)
	{
		temp = -temp;
	}
	
	return temp;
}
