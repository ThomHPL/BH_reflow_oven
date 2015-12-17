/*
 * PID.h
 *
 * Created: 16/12/2015 19:09:59
 *  Author: Thomas
 */ 


#ifndef PID_H_
#define PID_H_

void PID_start(unsigned int consigne,unsigned int periode);

void PID_stop();

void PID_setParams(unsigned int K,unsigned int Ti,unsigned int Td);

void PID_routine();

void PID_compute();

void PID_updateOutput();

#endif /* PID_H_ */