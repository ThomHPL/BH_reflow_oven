/*
 * RTC.h
 *
 * Created: 15-12-15 13:48:55
 *  Author: BOSTEM Antoine & HERPOEL Thomas
 */ 
// DS1307


#ifndef RTC_H_
#define RTC_H_

#define	DS1307_ADDR	0b1101000

/************************************************************************/
/* Indique l'heure sous la forme HH:MM:SS								*/
/* Tested OK															*/
/************************************************************************/
void RTC_getTime(char* str);

unsigned char* RTC_read(unsigned char deviceAddr,unsigned int nBytes, unsigned char byteAddr);

void read(unsigned char statusCode);

#endif /* RTC_H_ */
