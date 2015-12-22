// Button.h
#ifndef _Button_H_
#define _Button_H_

/*
    
    Bit             7   6   5   4   3   2   1   0
    ---------------------------------------------
    PORTD						L	D	E   	
	PORTB			R	U		

*/


// DEFINE

#define PIND_MASK ((1<<PIND4)|(1<<PIND3)|(1<<PIND2))
#define PINB_MASK ((1<<PINB6)|(1<<PINB7))
#define KEY_UP_PIN		PB6
#define KEY_RIGHT_PIN   PB7
#define KEY_ENTER_PIN   PD2
#define KEY_DOWN_PIN    PD3
#define KEY_LEFT_PIN    PD4


//Button definitions
#define KEY_NULL    0
#define KEY_UP		1
#define KEY_RIGHT   2
#define KEY_ENTER   3
#define KEY_DOWN    4
#define KEY_LEFT    5

// delai anti rebond
#define KEY_TMR		50

// PROTOTYPE FONCTIONS EXTERNES

/************************************************************************/
/* Initialise le clavier                                                */
/* Tested OK															*/
/************************************************************************/
void KEYBOARD_init(void);

/************************************************************************/
/* Retourne la dernière touche apuuyée                                  */
/* Tested OK															*/
/************************************************************************/
char KEYBOARD_getKey(void);


#endif /* _Button_H */



