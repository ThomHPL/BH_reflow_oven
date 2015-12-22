#ifndef _menu_h_
#define _menu_h_

#include "main.h"
#include "button.h"


//#ifndef PGM_P
//#define PGM_P const rom char * // pointeur vers un byte dans la ROM  
//#endif

// structure definitions
typedef struct PROGMEM
{
    unsigned char state;		// current state id
    PGM_P pText;				// pointer to the text
    char (*pFunc)(char input);	// pointer to the function
} MENU_STATE;					// structure state

typedef struct PROGMEM
{
    unsigned char state;		// current state ID
    unsigned char input;		// input to go to the next state
    unsigned char nextstate;	// next state ID
} MENU_NEXTSTATE;				// structure nextstate



// STATE IDS DEFINITION
// main menu
#define ST_WELCOME_ID				010
#define ST_TEMP_PTS_ID				020
	#define ST_TEMP_SETPALIER_ID		021
	#define ST_TEMP_SETPIC_ID			022
#define ST_PROFILES_ID				030
	#define ST_PROFILES_SAVE_ID			031
	#define ST_PROFILES_LOAD_ID			032
#define ST_RUN_ID					040

// manual menu
#define ST_MANUAL_ID				110
#define ST_MANUAL_TEMP_ID			120
	#define ST_MANUAL_SET_TEMP_ID		121
#define ST_MANUAL_CMD_ID			130
	#define ST_MANUAL_SET_CMD_ID		131

extern const char ST_WELCOME_TXT[];
extern const char ST_TEMP_PTS_TXT[];
extern const char ST_PROFILES_TXT[];
extern const char ST_PROFILES_SAVE_TXT[];
extern const char ST_PROFILES_LOAD_TXT[];
extern const char ST_RUN_TXT[];

extern const char ST_MANUAL_TXT[];
extern const char ST_MANUAL_TEMP_TXT[];
extern const char ST_MANUAL_CMD_TXT[];
extern const char ST_MANUAL_SET_CMD_TXT[];

extern const MENU_STATE Menu_State[];
extern const MENU_NEXTSTATE Menu_Nextstate[];

#endif /* _menu_h_ */