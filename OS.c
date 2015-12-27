// OS

#include "main.h"
#include "OS.h"
#include "lcd.h"
#include "button.h"
#include "menu.h"
#include "timers.h"

unsigned char OS_CURRENT_STATE;			// contient l'id de l'état courant
BOOL OS_first_run=TRUE;					// indique si on est à la première execution d'un état (utile pour certains états)

void (*TIMER_CB_FUNC[TIMER_CB_MAX])(void);
unsigned int TIMER_CB_TIME[TIMER_CB_MAX];
volatile unsigned int TIMER_CB_TICK[TIMER_CB_MAX];


void OS_init(void)
{
	
	// démarre le timer des callback
	CB_TIMER_Init_1ms();
	
	// Initialisation des callback
 	unsigned char i;
 	for (i=0; i<TIMER_CB_MAX; i++)
 	{
  		 TIMER_CB_FUNC[i] = 0;
  		 TIMER_CB_TIME[i] = 0;
		 TIMER_CB_TICK[i] = 0;
  	}

}

void OS_start()
{
	
	// for the state machine
	unsigned char nextstate;	// holds next state's id
	PGM_P statetext;			// point to the text to display
	char (*pStateFunc)(char);	// point to the state's function
	
	unsigned char input = KEY_NULL;		// holds pressed key
	//unsigned char i,j;
	
	// Init variables de la machine d'états
    OS_CURRENT_STATE = ST_WELCOME_ID;
    nextstate = OS_CURRENT_STATE;
	statetext = ST_WELCOME_TXT;
    pStateFunc = st_welcome;	
	
	// main loop
	while(TRUE)
	{
		
		/************************************************************************/
		/*						Gestion des callbacks							*/
		/************************************************************************/
		
		unsigned int j=1;					// Attention ,on commence à 1 (IDCB = 0 --> callback non enregistrée)
		while (TIMER_CB_FUNC[j]!=0 && j<TIMER_CB_MAX) j++;		// pour trouver le nombre de callback enregistrées
		
		unsigned char idx;
  		for (idx = 1; idx < j; idx++)		// pour chaque identificateur de callback
    	{
     		if (TIMER_CB_FUNC[idx] &&
				TIMER_CB_TICK[idx] >= TIMER_CB_TIME[idx])		// si on est arrivé au nombre de ms demandé 
      		{
				TIMER_CB_TICK[idx] = 0;
      			TIMER_CB_FUNC[idx]();		// Appel de la fonction enregistrée!					
	 		}
  		}
		
		/************************************************************************/
		/*					Gestion de la machine d'états						*/
		/************************************************************************/

		// 1 - Lecture des boutons
		input = KEYBOARD_getKey();
		
		// 2 - Affiche le texte lié à l'état
		if (statetext) 
		{
			cli();
			lcd_clrscr();
			lcd_gotoxy(0,0);
			lcd_puts_p(statetext);
			sei();
			statetext = NULL;
		}
		
		// 3 - Appel de la fonction liée à l'état    
		if (pStateFunc)
		{
	  		nextstate = pStateFunc(input);		// la fonction retourne le prochain état
			OS_first_run=FALSE;					// ce n'est plus la première execution de l'état
		}	
		else if(input!=KEY_NULL)
		{
			nextstate = OS_stateMachine(OS_CURRENT_STATE, input);
		}
		
		// 4 - Si l'état change...
		if (nextstate != OS_CURRENT_STATE) 
		{
			OS_first_run=TRUE;					// ce sera la première execution de l'état
			OS_CURRENT_STATE = nextstate;		// l'état est maintenant le nouvel état de la séquence définie dans le menu
			unsigned char i;
			for (i=0; (j=pgm_read_byte(&Menu_State[i].state)); i++)
			{
				if (j == OS_CURRENT_STATE)
				{
					statetext =  (PGM_P) pgm_read_word(&Menu_State[i].pText);
					pStateFunc = (PGM_VOID_P) pgm_read_word(&Menu_State[i].pFunc);
				}
			}
		}
	}
}

unsigned char OS_stateMachine(char state, unsigned char stimuli)
{
	// reste dans le meme état par défaut
    unsigned char nextstate = state;
	
	// parcours tous les états
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


unsigned char OS_addCallback(void(*ptFonction)(void), unsigned int period)
{
	// Attention, on commence à 1 (IDCB = 0 --> callback non enregistrée)
	unsigned int i=1;
	
 	while (TIMER_CB_FUNC[i]!=0 && i<TIMER_CB_MAX) i++;
	
	// S'il reste de la place on enregistre et on retourne l'id du callback
 	if (i<TIMER_CB_MAX)
 	{
  		 TIMER_CB_FUNC[i] = ptFonction;
  		 TIMER_CB_TIME[i] = period;
  		 TIMER_CB_TICK[i] = 0; //Initialiser le compteur à 0;
   	     return i; // ID du call back
  	}
 	else return 0; // Retourne 0 si il n'y a plus de place pour enregistrer un callback
}

unsigned char OS_removeCallback(unsigned char CB_ID)
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
	while (TIMER_CB_FUNC[j]!=0 && j<TIMER_CB_MAX) j++; // sert à trouver le nombre de callback enregistrées
	
	unsigned char i;
  	for (i = 0; i < j; i++) TIMER_CB_TICK[i]++; // pour chaque callback enregistrée, on incrémente le tick

	sei();
}

