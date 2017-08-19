#include "retro.h"

void retroInit(void) {
	mainRetro = FRONT;
}

void badRetro(void){

	if (&mainRetro.count == &FRONT.count) {
		if (FRONT.count != MIDDLE.count && MIDDLE.count == REAR.count && MIDDLE.count > FRONT.count)
			mainRetro = MIDDLE;
	}
	else if (&mainRetro.count == &MIDDLE.count) {
		if (MIDDLE.count < REAR.count)
			mainRetro = REAR;
	}
	else {
		if (FRONT.count > REAR.count && FRONT.count > MIDDLE.count)
			mainRetro = FRONT;
		else if (MIDDLE.count > REAR.count && MIDDLE.count > FRONT.count)
			mainRetro = MIDDLE;
	}

}

int getVelocity(void) {
	int velocity;

	badRetro();
	
	velocity = 100.0/MAIN_INTERVAL; /*feet/ms*/
	velocity = velocity * 30 + velocity / 2.0; /*cm/ms*/
	velocity = velocity/1000.0; /*cm/s*/

	return velocity;
}

