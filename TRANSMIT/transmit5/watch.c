#include "watch.h"
#include "timer_interrupts.h"


                  



typedef struct  { 

	unsigned char ucMinutes; 
	unsigned char	ucSecconds; 
	bool fSeccondsValueChanged ;
	bool fMinutesValueChanged;

}Watch;

#define WATCH_INIT_CONFIG {                       \
			.ucMinutes=0,  															\ 
			.ucSecconds=0,                              \
			.fSeccondsValueChanged=0,                   \
			.fMinutesValueChanged=0,                    \
}   

const Watch sWatchDefaultConfig= {
			.ucMinutes=0, 															 
			.ucSecconds=0,                              
			.fSeccondsValueChanged=false,                  
			.fMinutesValueChanged=false,
};

/*private variables*/
static Watch sWatch;

/*private functions*/
static void WatchUpdate(void){
		sWatch.ucSecconds++;
	if(sWatch.ucSecconds>59){
		sWatch.ucSecconds=0;
		sWatch.ucMinutes++;
	  sWatch.fMinutesValueChanged=1;
	}
	sWatch.fSeccondsValueChanged=1;
}

/*public functions*/
void WatchInit(void){
  sWatch = sWatchDefaultConfig;
	Timer0Interrupts_Init(100000, WatchUpdate);
}

unsigned char WatchGetSeconds(void){
	return sWatch.ucSecconds;
}

unsigned char WatchGetMinutes(void){
	return sWatch.ucMinutes;
}


bool WatchAreSecondsChanged(void){
	if(sWatch.fSeccondsValueChanged){
     sWatch.fSeccondsValueChanged=false;
		 return true;
	}
	return false;
}

bool WatchAreMinutesChanged(void){
	if(sWatch.fMinutesValueChanged){
     sWatch.fMinutesValueChanged=false;
		 return true;
	}
	return false;
}
