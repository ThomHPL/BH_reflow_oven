/*
 * EEPROM.h
 *
 * Created: 09/12/2015 18:13:10
 *  Author: Thomas
 */ 


#ifndef EEPROM_H_
#define EEPROM_H_



void EEPROM_writeByte(unsigned char data,unsigned char deviceAddr,unsigned int byteAddr);
void EEPROM_writePage(unsigned char* data,unsigned char deviceAddr,unsigned int pageAddr,unsigned char pageSize);



#endif /* EEPROM_H_ */