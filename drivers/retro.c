#include "exti.h"
#include "retro.h"


extern timeStamp * interLine;

timeStamp mainRetro;
volatile unsigned int *main_count = NULL;

void retroInit(void) {
	mainRetro = FRONT;
}

int getVelcity(void) {
	int vel, time;
	time = mainRetro.curr - mainRetro.prev;

	/* feet per milisecond */
	vel= 100/time;

	vel = vel * 

		
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
