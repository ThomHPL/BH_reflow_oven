// Fonctions liées au joystick

//INCLUDE
#include "main.h"
#include "button.h"
#include "OS.h"

//DECLARATION DES VARIABLES GLOGALES
volatile char KEY = NULL;
char CBID_ReadKey = 0;
char CBID_KEYDOWN_simulation = 0;

//PROTOTYPE FONCTIONS INTERNES
void read_key(void);

//CONTENU FONCTIONS EXTERNES
void KEYBOARD_init(void)
{
    // Init port pins
	cbiBF(DDRD,PD2);
	cbiBF(DDRD,PD3);
	cbiBF(DDRD,PD4);
	cbiBF(DDRB,PB6);
	cbiBF(DDRB,PB7);

	PORTD |= PIND_MASK;		// PORTC to 1 => internal pull up
	PORTB |= PINB_MASK;
	
	MCUCR &= 0<<PUD;		// To ensure pull ups are not disabled
	
    // Enable pin change interrupt on PORTC
	PCMSK2 |= (1<<PCINT20)|(1<<PCINT19)|(1<<PCINT18);
    PCMSK0 |= (1<<PCINT7)|(1<<PCINT6);
	
	// Enable interrupts
    PCICR |= (1<<PCIE2)|(1<<PCIE0);
	
}


ISR(PCINT2_vect)
{
	cli();
	cbiBF(PCICR,PCIE2);
	cbiBF(PCICR,PCIE0);  
	CBID_ReadKey = OS_addTimerCallback(read_key,10);
	sei();
}
ISR(PCINT0_vect)
{
	cli();
	cbiBF(PCICR,PCIE2); 
	cbiBF(PCICR,PCIE0); 
	CBID_ReadKey = OS_addTimerCallback(read_key,10);
	sei();
}

void read_key(void)
{
	CBID_ReadKey=OS_removeTimerCallback(CBID_ReadKey);
	
    char buttonsD = 0;
	char buttonsB = 0;
    char key;
    buttonsD = ((~PIND) & PIND_MASK );
	buttonsB = ((~PINB) & PINB_MASK ) ;

    if (buttonsB & (1<<KEY_UP_PIN))
        key = KEY_UP;
    else if (buttonsD & (1<<KEY_DOWN_PIN))
        key = KEY_DOWN;
    else if (buttonsD & (1<<KEY_LEFT_PIN))
        key = KEY_LEFT;
    else if (buttonsB & (1<<KEY_RIGHT_PIN))
        key = KEY_RIGHT;
    else if (buttonsD & (1<<KEY_ENTER_PIN))
        key = KEY_ENTER;
    else
        key = KEY_NULL;
	
	
	if(KEY==key)
		KEY = KEY_NULL;
	else
		KEY = key;
	
	sbiBF(PCICR,PCIE2);
	sbiBF(PCICR,PCIE0);
	
}

char KEYBOARD_getKey(void)
{
	char k=KEY;
	KEY=KEY_NULL;
    return k;
}
