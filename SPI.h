/*
 * SPI.h
 *
 * Created: 16/11/2015 18:03:40
 *  Author: BOSTEM Antoine & HERPOEL Thomas
 */ 


#ifndef SPI_H_
#define SPI_H_

#define SPI_PORT	PORTB
#define SPI_DDR		DDRB
#define SPI_SCK		5
#define SPI_MISO	4
#define SPI_MOSI	3
#define SPI_SS		2

// #define SPI_NBITS	32
#define SPI_CPHA	0
#define SPI_CPOL	0

// pull SS low
// when 8 bits are shifted, end of transmission flag (SPIF) and clock stop
// if SPIE in SPICR is set, interrupt

// in interrupt:
// 1 -

/************************************************************************/
/* Initialise le SPI pour lire une frame de nb bits						*/
/* Tested OK															*/
/************************************************************************/
void SPI_init(unsigned char nb);
/************************************************************************/
/* Démarre la lecture du SPI                                            */
/* Tested OK															*/
/************************************************************************/
void SPI_start();
/************************************************************************/
/* Renvoie le frame de nb bits (max 32 bits)                            */
/* Tested OK															*/
/************************************************************************/
uint32_t SPI_getFrame();

#endif /* SPI_H_ */