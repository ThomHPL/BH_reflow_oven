/*
 * I2C.h
 *
 * Created: 09/12/2015 07:57:21
 *  Author: Thomas
 */ 


#ifndef I2C_H_
#define I2C_H_

#define CPU_F		8000000
#define PRESCALER	1

// define prescaler
#define DIV_1		0b00
#define DIV_4		0b01
#define DIV_16		0b10
#define DIV_64		0b11

// define status codes

// master mode
#define M_START_ACK			0x08
#define M_REP_START_ACK		0x10
#define M_SLAW_ACK			0x18
#define M_SLAW_NACK			0x20
#define M_DATA_TX_ACK		0x28
#define M_DATA_TX_NACK		0x30
#define M_ARBR_LOST			0x38
#define M_SLAR_ACK			0x40
#define M_SLAR_NACK			0x48
#define M_DATA_RX_ACK		0x50
#define M_DATA_RX_NACK		0x58

// slave mode
// TODO


extern unsigned char SPI_statusCode;

/************************************************************************/
/* Init of the SPI controller at the choosen clock frequency            */
/************************************************************************/
void I2C_init(int sclKHz);

/************************************************************************/
/* Sets the address SLA of the device to use                            */
/************************************************************************/
void I2C_setAddress(unsigned char addr);

/************************************************************************/
/* Sets the function to do in interrupt		                            */
/************************************************************************/
void I2C_setFunction(void *func(unsigned char));

/************************************************************************/
/* Sends a START condition on the bus                                   */
/************************************************************************/
void I2C_start(void);

/************************************************************************/
/* Sends a STOP condition on the bus                                    */
/************************************************************************/
void I2C_stop(void);

/************************************************************************/
/* Sends a byte                                                         */
/************************************************************************/
void I2C_send(unsigned char data);

/************************************************************************/
/* Sends the 7 bits SLA + write bit                                     */
/************************************************************************/
void I2C_sendSLAW();

/************************************************************************/
/* Sends the 7 bits SLA + read bit                                      */
/************************************************************************/
void I2C_sendSLAR();




#endif /* I2C_H_ */