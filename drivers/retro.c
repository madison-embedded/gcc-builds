#include "retro.h"
#include "badgerloop.h"

void initRetro(void) {
	mainRetro = &FRONT;
}

void badRetro(void){

	if (&mainRetro->count == &FRONT.count) {
		if (FRONT.count != MIDDLE.count && MIDDLE.count == REAR.count && MIDDLE.count > FRONT.count)
			mainRetro = &MIDDLE;
	}
	else if (&mainRetro->count == &MIDDLE.count) {
		if (MIDDLE.count < REAR.count)
			mainRetro = &REAR;
	}
	else {
		if (FRONT.count > REAR.count && FRONT.count > MIDDLE.count)
			mainRetro = &FRONT;
		else if (MIDDLE.count > REAR.count && MIDDLE.count > FRONT.count)
			mainRetro = &MIDDLE;
	}

}

int getVelocity(void) {
	float velocity;

	badRetro();
	if (!MAIN_INTERVAL) return 0;
	if (CHECK_THRESHOLD(MAIN_INTERVAL, 203, 0)) {
		return 15000; /* TODO may subtract stip */
	}
	velocity = 3048000 / MAIN_INTERVAL;

	return velocity;
}

