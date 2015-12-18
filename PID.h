/*
 * PID.h
 *
 * Created: 16/12/2015 19:09:59
 *  Author: Thomas
 */ 


#ifndef PID_H_
#define PID_H_

void PID_start(int consigne,int periode);

void PID_stop();

void PID_setParams(float K,float Ti,float Td);

void PID_routine();

void PID_compute();

void PID_updateOutput();

#endif /* PID_H_ */