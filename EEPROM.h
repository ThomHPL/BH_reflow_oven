/*
 * EEPROM.h
 *
 * Created: 09/12/2015 18:13:10
 *  Author: BOSTEM Antoine & HERPOEL Thomas
 */ 


#ifndef EEPROM_H_
#define EEPROM_H_


/************************************************************************/
/* Read a page at the given address										*/
/* TESTED OK															*/
/************************************************************************/
unsigned char* EEPROM_readPage(unsigned char deviceAddr,unsigned int pageAddr, unsigned char pageSize);

/************************************************************************/
/* Write a page at the given address                                    */
/* TESTED OK															*/
/************************************************************************/
void EEPROM_writePage(unsigned char* data,unsigned char deviceAddr,unsigned int pageAddr,unsigned char pageSize);

/************************************************************************/
/* TESTED OK                                                            */
/************************************************************************/
void EEPROM_erasePage(unsigned char deviceAddr,unsigned int pageAddr,unsigned char pageSize);


#endif /* EEPROM_H_ */