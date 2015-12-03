// USART.h

#ifndef _USART_H_
#define _USART_H_

//INCLUDE

//DEFINE
// F_CPU (fréquencehorloge du µC) est connu par le compilateur (lors de la configuration du projet)
// La formule du BAUD_RATE est renseignée dans la DS du 8088 (page 174)
// Asynchronous normal mode (U2X0=0)
//#define UART_BAUD_RATE 							9600L		//9600 bauds
//#define UART_BAUD_CALC(UART_BAUD_RATE,F_CPU)	((F_CPU)/((UART_BAUD_RATE)*16L)-1)

//PROTOTYPE FONCTIONS EXTERNES
void RS232_init(int baudrate);
void RS232_sendByte(unsigned char data);
void RS232_sendBuffer(char* buffer, int bufferSize);
void RS232_print(char* str);
void RS232_println(char* str);



#endif /* _USART_H */
