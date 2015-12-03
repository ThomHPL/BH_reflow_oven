// OS

#include "main.h"
#include "OS.h"
#include "lcd.h"
#include "button.h"
#include "menu.h"
#include "timers.h"

unsigned char OS_CURRENT_STATE;			// holds the current state id


void (*TIMER_CB_FUNC[TIMER_CB_MAX])(void);
unsigned int TIMER_CB_TIME[TIMER_CB_MAX];
volatile unsigned int TIMER_CB_TICK[TIMER_CB_MAX];


void OS_init(void)
{
	
 	unsigned char i;
	
 	//Initialisation pour variables CallBack
 	for (i=0; i<TIMER_CB_MAX; i++)
 	{
  		 TIMER_CB_FUNC[i] = 0;
  		 TIMER_CB_TIME[i] = 0;
		 TIMER_CB_TICK[i] = 0;
  	}	

}

void OS_start()
{
	// start the callback timer
	CB_TIMER_Init_1ms();

	// for the CALLBACKS
	unsigned char idx;
	
	// for the state machine
	unsigned char nextstate;	// holds next state's id
	PGM_P statetext;			// point to the text to display
	char (*pStateFunc)(char);	// point to the state's function
	
	unsigned char input = KEY_NULL;		// holds pressed key
	unsigned char i,j;
	
	// Init variables de la machine d'états
    OS_CURRENT_STATE = ST_WELCOME_ID;
    nextstate = OS_CURRENT_STATE;
	statetext = ST_WELCOME_TXT;
    pStateFunc = st_welcome;
	
	// main loop
	while(TRUE)
	{
		// Gestion des callback
		unsigned int j=1; //Attention ,on commence à 1 (IDCB = 0 --> callback non enregistrée)
		while (TIMER_CB_FUNC[j]!=0 && j<TIMER_CB_MAX) j++; // pour trouver le nombre de callback enregistrées
		
		// Check les conditions pour rappeler les fonctions liées au temps
  		for (idx = 1; idx < j; idx++)
    	{
	 		if (TIMER_CB_FUNC[idx]) //Si on a l'adresse d'une fonction CB à cet index
     			if (TIMER_CB_TICK[idx] >= TIMER_CB_TIME[idx])//Si on est arrivé au nombre de mS demandé, on appelle la fonction 
      			{ 
	  				//TIMER_CB_TICK[idx] = TIMER_CB_TICK[idx] - TIMER_CB_TIME[idx];
					TIMER_CB_TICK[idx] = 0;
      				TIMER_CB_FUNC[idx]();  //Appel de la fonction enregistrée!					
	 			}
  		}
		
		//Gestion machine d'état
		
		// 1 - Read buttons
		input = KEYBOARD_getKey();
		
		// 2 - display available state text
		if (statetext) 
		{
			cli();
			lcd_clrscr();
			lcd_gotoxy(0,0);
			lcd_puts_p(statetext);
			sei();
			statetext = NULL;
		}
		
		// 3 - Call available state function         
		if (pStateFunc)
		{
	  		nextstate = pStateFunc(input);
		}	
		else
		{
			nextstate = OS_stateMachine(OS_CURRENT_STATE, input);
		}
		 
		if (nextstate != OS_CURRENT_STATE)  // il y a changement d'état 
		{
			OS_CURRENT_STATE = nextstate; // l'état est maintenant le nouvel état de la séquence définie dans main.h
			for (i=0; (j=pgm_read_byte(&Menu_State[i].state)); i++)
			{
				if (j == OS_CURRENT_STATE)
				{
					statetext =  (PGM_P) pgm_read_word(&Menu_State[i].pText);
					pStateFunc = (PGM_VOID_P) pgm_read_word(&Menu_State[i].pFunc);
					//break;
				}
			}
		}		
		
	}     
}

unsigned char OS_stateMachine(char state, unsigned char stimuli)
{
    unsigned char nextstate = state;    // Default stay in same state
    unsigned char i, j;
    for (i=0; ( j=pgm_read_byte(&Menu_Nextstate[i].state) ); i++ )
    {
        if ( j == state && 
             pgm_read_byte(&Menu_Nextstate[i].input) == stimuli)
        {
            // This is the one!
            nextstate = pgm_read_byte(&Menu_Nextstate[i].nextstate);
            break;
        }
    }
    return nextstate;
}


unsigned char OS_addTimerCallback(void(*ptFonction)(void), unsigned int period)
{
	unsigned int i=1; //Attention ,on commence à 1 (IDCB = 0 --> callback non enregistrée)
	 
 	while (TIMER_CB_FUNC[i]!=0 && i<TIMER_CB_MAX) i++;
	
	// S'il reste de la place on enregistre et on retourne l'id du callback
 	if (i<TIMER_CB_MAX)
 	{
  		 TIMER_CB_FUNC[i] = ptFonction;
  		 TIMER_CB_TIME[i] = period; 
  		 TIMER_CB_TICK[i] = 0; //Initialiser le compteur à 0;
   	     return i; // ID du call back
  	}
 	else return 255; //Il n'y a plus de place pour enregistrer un callback
}

unsigned char OS_removeTimerCallback(unsigned char CB_ID)
{
   	TIMER_CB_FUNC[CB_ID] = 0;
  	TIMER_CB_TIME[CB_ID] = 0;
	TIMER_CB_TICK[CB_ID] = 0;
	return 0;
}

unsigned char OS_updateCallbackTime(unsigned char CB_ID,unsigned int time)
{
	TIMER_CB_TIME[CB_ID] = time;
}	

ISR(TIMER2_COMPA_vect)
{
	cli();
	
	unsigned int j=1; //Attention ,on commence à 1 (IDCB = 0 --> callback non enregistrée)
	while (TIMER_CB_FUNC[j]!=0 && j<TIMER_CB_MAX) j++;
	
	unsigned char i;
  	for (i = 0; i < j; i++) TIMER_CB_TICK[i]++;

	sei();
}

