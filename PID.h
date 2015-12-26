/*
 * PID.h
 *
 * Created: 16/12/2015 19:09:59
 *  Author: BOSTEM Antoine & HERPOEL Thomas
 */ 


#ifndef PID_H_
#define PID_H_

/************************************************************************/
/* Démarre la régulation                                                */
/************************************************************************/
void PID_start();

void PID_setConsigne(int consigne);

void PID_stop();

void PID_setParams(float K,float Ti,float Td);

void PID_routine();

void PID_compute();

void PID_updateOutput();

#endif /* PID_H_ */