// main.h

#ifndef _main_h_
#define _main_h_

//INCLUDE

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

//DEFINE
#define BOOL    char
#define TRUE	1
#define FALSE	0
#define NULL	0

extern unsigned char PWM_DC;

// Gestion registre par bit unique
#define sbiBF(port,bit)  (port |= (1<<bit))   //set bit in port
#define cbiBF(port,bit)  (port &= ~(1<<bit))  //clear bit in port

//PROTOTYPE FONCTIONS EXTERNES
unsigned char StateMachine(char state, unsigned char stimuli);

// main menu
char st_welcome(char input);
char st_temp_pts(char input);
char st_profiles(char input);
	char st_profiles_save(char input);
	char st_profiles_load(char input);
char st_run(char input);

// manual menu
char st_manual(char input);
char st_manual_temp(char input);
char st_manual_cmd(char input);
	char st_manual_set_cmd(char input);
	
enum {IDLE,PREHEAT,SOAK,DWELL,COOLING};

// other functions

/*
char blink_tgl(char input);
char blink_period(char input);
char displayTemp(char input);
char spiFrame(char input);
char setCommand(char input);
*/

#endif /* _main.h */