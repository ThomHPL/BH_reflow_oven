/*
 * MAX.h
 *
 * Created: 25/11/2015 14:06:34
 *  Author: BOSTEM Antoine & HERPOEL Thomas
 */ 

#ifndef MAX_H_
#define MAX_H_

#define MAX_31855
//#define MAX_6675

// define en fonction du capteur utilisé (voir datasheets)
#ifdef MAX_31855
#define FRAME_SIZE	32
#define TEMP_POS	18
#define TEMP_SIZE	14

#else 

#ifdef MAX_6675
#define FRAME_SIZE	16
#define TEMP_POS	3
#define TEMP_SIZE	12
#endif

#endif

/************************************************************************/
/* initialise le capteur                                                */
/* tested OK															*/
/************************************************************************/
void MAX_init(void);
/************************************************************************/
/* démarre la mesure													*/
/* tested OK															*/
/************************************************************************/
void MAX_start(void);
/************************************************************************/
/* renvoie la température mesurée										*/
/* tested OK															*/
/************************************************************************/
int MAX_getTemp(void);


#endif /* MAX31855_H_ */