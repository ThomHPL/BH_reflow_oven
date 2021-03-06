// USART.h

#ifndef _USART_H_
#define _USART_H_

//PROTOTYPE FONCTIONS EXTERNES
void RS232_init(int baudrate);
void RS232_sendByte(unsigned char data);
void RS232_sendBuffer(unsigned char* buffer, int bufferSize);
void RS232_print(char* str);
void RS232_println(const char* str);
void RS232_println_debug(char* str);

#endif /* _USART_H */
