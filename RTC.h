/*
 * RTC.h
 *
 * Created: 15-12-15 13:48:55
 *  Author: Antoine
 */ 
// DS1307


#ifndef RTC_H_
#define RTC_H_

#define	DS1307_ADDR	0b1101000

char* RTC_getTime(void);

unsigned char* RTC_read(unsigned char deviceAddr,unsigned int nBytes, unsigned char byteAddr);

void read(unsigned char statusCode);

#endif /* RTC_H_ */

/*
#define		SECONDS			rtc_register[0]
#define		MINUTES			rtc_register[1]
#define		HOURS			rtc_register[2]
#define		DAY    			rtc_register[3]
#define		DATE   			rtc_register[4]
#define		MONTH  			rtc_register[5]
#define		YEAR   			rtc_register[6]


char rtc_register[7];
char time[8]; 		//xx:xx:xx;
char date[10];		//xx/xx/xxxx;
char day;


void RTC_setStartAddress(void);

unsigned char RTC_read(void);

void RTC_getTime(void);

void RTC_getDate(void);

char RTC_displayTime(char);

void RTC_updateRegisters(void);

unsigned char RTC_writeTime(void);

char RTC_updateTime(void);

void RTC_updateDate(void);

*/