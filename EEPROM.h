/*
 * EEPROM.h
 *
 * Created: 09/12/2015 18:13:10
 *  Author: Thomas
 */ 


#ifndef EEPROM_H_
#define EEPROM_H_

unsigned char EEPROM_readByte(unsigned char deviceAddr,unsigned int byteAddr);

/************************************************************************/
/* Read a page at the given address										*/
/* TESTED OK															*/
/************************************************************************/
unsigned char* EEPROM_readPage(unsigned char deviceAddr,unsigned int pageAddr, unsigned char pageSize);

/************************************************************************/
/* Write a byte at the given address                                    */
/* TESTED OK															*/
/************************************************************************/
//char EEPROM_writeByte(unsigned char* data,unsigned char deviceAddr,unsigned int byteAddr);

/************************************************************************/
/* Write a page at the given address                                    */
/* TESTED OK															*/
/************************************************************************/
char EEPROM_writePage(unsigned char* data,unsigned char deviceAddr,unsigned int pageAddr,unsigned char pageSize);

/************************************************************************/
/* Write a page at the given address                                    */
/* TESTED OK															*/
/************************************************************************/
char EEPROM_erasePage(unsigned char deviceAddr,unsigned int pageAddr,unsigned char pageSize);



#endif /* EEPROM_H_ */