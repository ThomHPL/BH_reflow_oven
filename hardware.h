/*
 * hardware.h
 *
 * Created: 30/10/2015 16:41:11
 *  Author: Thomas
 */ 


#ifndef HARDWARE_H_
#define HARDWARE_H_

#define PIN_STATUS_LED			PD5
#define PORT_STATUS_LED			PORTD

void hardware_init(void);
void statusLed_init(void);
void statusLedOn(void);
void statusLedOff(void);
void statusLedToggle(void);


#endif /* HARDWARE_H_ */