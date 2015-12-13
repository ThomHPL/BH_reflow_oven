/*
 * EEPROM.h
 *
 * Created: 09/12/2015 18:13:10
 *  Author: Thomas
 */ 


#ifndef EEPROM_H_
#define EEPROM_H_


/************************************************************************/
/* Write a byte at the given address                                    */
/* sTESTED OK															*/
/************************************************************************/
void EEPROM_writeByte(unsigned char* data,unsigned char deviceAddr,unsigned int byteAddr);

/************************************************************************/
/* Write a page at the given address                                    */
/* TESTED OK															*/
/************************************************************************/
void EEPROM_writePage(unsigned char* data,unsigned char deviceAddr,unsigned int pageAddr,unsigned char pageSize);

/************************************************************************/
/* Write a page at the given address                                    */
/* TESTED OK															*/
/************************************************************************/
void EEPROM_erasePage(unsigned char deviceAddr,unsigned int pageAddr,unsigned char pageSize);



#endif /* EEPROM_H_ */