#include "retro.h"
#include "badgerloop.h"

void initRetro(void) {
	mainRetro = &FRONT;
}

void badRetro(void){
	if (FRONT.count == MIDDLE.count)
		mainRetro = &FRONT;
	else if (MIDDLE.count == REAR.count)
		mainRetro = &MIDDLE;
	else if (FRONT.count == REAR.count)
		mainRetro = &FRONT;
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

