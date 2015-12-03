//FONCTIONS USART


//INCLUDE
#include "USART.h"
#include "Main.h"
#include <avr/io.h>

//DECLARATION DES VARIABLES GLOGALES


//PROTOTYPE FONCTIONS INTERNES


//CONTENU FONCTIONS EXTERNES
void RS232_init(int baudrate)
{
// fréquence horloge = 8000000 hz, Si Baudrate = 9600 alors UBRR = 103
	//double speed  U2X0 = 1  
	UCSR0A |= (1<<U2X0);
	
	// UBRR0 = F_CPU/(8*BAUD)-1
	//int32_t cpuFrequency = 8000000;
	//UBRR0 = cpuFrequency/(8*baudrate)-1;
	// 9600 baud
	UBRR0 = 103;

	// Configuration en émission seulement.
	UCSR0B |= (0<<RXCIE0)|(0<<TXCIE0)|(0<<UDRIE0)|(0<<RXEN0)|(1<<TXEN0)|(0<<UCSZ02)|(0<<RXB80)|(0<<TXB80);

	// Async. mode, 8 bits, 1 bit de stop, pas de contrôle de parité
   	UCSR0C |= (0<<UMSEL00)|(0<<UPM01)|(0<<UPM00)|(0<<USBS0)|(1<<UCSZ01)|(1<<UCSZ00)|(0<<UCPOL0);
}

void RS232_sendByte(unsigned char data)
{
    // UDRE Flag , is the transmit buffer UDR) ready to receive new data ?
	// if UDRE0 =1 the buffer is empty
	while (!(UCSR0A & (1<<UDRE0)));
    UDR0 = data;
}

void RS232_print(char* str)
{
	while(*str!=0)
	{
		RS232_sendByte(*str++);		
	}
}

void RS232_println(char* str)
{
	while(*str!=0)
	{
		RS232_sendByte(*str++);	
	}
	RS232_print("\r\n");
}

void RS232_sendBuffer(char* buffer, int bufferSize)
{
	for(int i=0;i<bufferSize;i++)
	{
		RS232_sendByte(*buffer++);
	}
}



//CONTENU FONCTIONS INTERNES

