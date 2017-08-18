#include "exti.h"

#define FRONT 	interline[9];
#define MIDDLE 	interline[11];
#define REAR	interline[13];

extern timeStamp * interLine;

timeStamp mainRetro;
volatile unsigned int *main_count = NULL;

void retroInit(){
	mainRetro = FRONT;
}

int getVelcity(void) {
	int vel, time;
	time = interLine[0].curr - interLine[0].prev;

	/* feet per milisecond */
	vel= 100/time; 

	
	return vel;

}

LOCAL int badRetro(void){

	if (mainRetro.count == &FRONT.count) {
		if (FRONT.count != MIDDLE.count && MIDDLE.count == REAR.count && MIDDLE.count > FRONT.count)
			mainRetro = MIDDLE;
	}
	else if (main.count == &MIDDLE.count) {
		if (MIDDLE.count < REAR.count)
			mainRetro = REAR;
	}
	else {
		if (FRONT.count > REAR.count && FRONT.count > MIDDLE.count)
			mainRetro = FRONT;
		else if (MIDDLE.count > REAR.count && MIDDLE.count > FRONT.count)
			mainRetro = MIDDLE;
	}

	return 1;
}


