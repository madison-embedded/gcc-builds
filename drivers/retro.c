#include "retro.h"
#include "badgerloop.h"

void initRetro(void) {
	mainRetro = &FRONT;
}

void badRetro(void){
	if (FRONT.count == MIDDLE.count) {
		if (mainRetro != &FRONT) printf("main retro now front\r\n");
		mainRetro = &FRONT;
	} else if (MIDDLE.count == REAR.count) {
		if (mainRetro != &FRONT) printf("main retro now middle\r\n");
		mainRetro = &MIDDLE;
	} else if (FRONT.count == REAR.count) {
		if (mainRetro != &FRONT) printf("main retro now front\r\n");
		mainRetro = &FRONT;
	}
}

int getVelocity(void) {
	int velocity;

	//badRetro();
	__disable_irq();
	if (!MAIN_INTERVAL) return 0;
	velocity = (1000 * CM_PER_STRIP) / MAIN_INTERVAL;
	__enable_irq();

	return velocity;
}

