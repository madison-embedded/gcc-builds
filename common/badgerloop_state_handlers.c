#include "badgerloop.h"
#include "state_machine.h"
#include "config.h"
#include <stdio.h>

const unsigned int state_intervals[] = {
	2500, /* FAULT		*/
	2500, /* IDLE		*/
	2500, /* READY		*/
	2500, /* PUSHING	*/
	2500, /* COAST		*/
	2500  /* BRAKING	*/
};

/*****************************************************************************/
/*                              Fault Handlers                               */
/*****************************************************************************/
void to_fault(STATE_NAME from, uint32_t flags) {
	printf("executing %s from %s\r\n", __func__, state_strings[from]);
}

void in_fault(uint32_t flags) {
	if (check_interval(FAULT)) {
		print_time();
		printf("curr fault: %s\r\n", fault_message);
	}
}
void from_fault(STATE_NAME to, uint32_t flags) {
	printf("executing %s to %s\r\n", __func__, state_strings[to]);
}
/*****************************************************************************/
/*****************************************************************************/


/*****************************************************************************/
/*                                 Idle Handlers                             */
/*****************************************************************************/
void to_idle(STATE_NAME from, uint32_t flags) {
	printf("executing %s from %s\r\n", __func__, state_strings[from]);
}

void in_idle(uint32_t flags) {
	if (check_interval(IDLE)) {
		print_time();
		printf("%s\r\n", __func__);
	}
}

void from_idle(STATE_NAME to, uint32_t flags) {
	printf("executing %s to %s\r\n", __func__, state_strings[to]);
}
/*****************************************************************************/
/*****************************************************************************/


/*****************************************************************************/
/*                             Ready Handlers                                */
/*****************************************************************************/
void to_ready(STATE_NAME from, uint32_t flags) {
	printf("executing %s from %s\r\n", __func__, state_strings[from]);
}

void in_ready(uint32_t flags) {
	if (check_interval(READY)) {
		print_time();
		printf("%s\r\n", __func__);
	}
}

void from_ready(STATE_NAME to, uint32_t flags) {
	printf("executing %s to %s\r\n", __func__, state_strings[to]);
}
/*****************************************************************************/
/*****************************************************************************/


/*****************************************************************************/
/*                               Pushing Handlers                            */
/*****************************************************************************/
void to_pushing(STATE_NAME from, uint32_t flags) {
	printf("executing %s from %s\r\n", __func__, state_strings[from]);
}

void in_pushing(uint32_t flags) {
	if (check_interval(PUSHING)) {
		print_time();
		printf("%s\r\n", __func__);
	}
}

void from_pushing(STATE_NAME to, uint32_t flags) {
	printf("executing %s to %s\r\n", __func__, state_strings[to]);
}
/*****************************************************************************/
/*****************************************************************************/


/*****************************************************************************/
/*                                 Coast Handlers                            */
/*****************************************************************************/
void to_coast(STATE_NAME from, uint32_t flags) {
	printf("executing %s from %s\r\n", __func__, state_strings[from]);
}

void in_coast(uint32_t flags) {
	if (check_interval(COAST)) {
		print_time();
		printf("%s\r\n", __func__);
	}
}

void from_coast(STATE_NAME to, uint32_t flags) {
	printf("executing %s to %s\r\n", __func__, state_strings[to]);
}
/*****************************************************************************/
/*****************************************************************************/


/*****************************************************************************/
/*                               Braking Handlers                            */
/*****************************************************************************/
void to_braking(STATE_NAME from, uint32_t flags) {
	printf("executing %s from %s\r\n", __func__, state_strings[from]);
}

void in_braking(uint32_t flags) {
	if (check_interval(BRAKING)) {
		print_time();
		printf("%s\r\n", __func__);
	}
}

void from_braking(STATE_NAME to, uint32_t flags) {
	printf("executing %s to %s\r\n", __func__, state_strings[to]);
}
/*****************************************************************************/
/*****************************************************************************/

unsigned int state_event_timestamps[NUM_STATES];

state_transition_t * const to_handlers[] = {
	to_fault, to_idle, to_ready, to_pushing, to_coast, to_braking
};

state_handler_t * const in_handlers[] = {
	in_fault, in_idle, in_ready, in_pushing, in_coast, in_braking
};

state_transition_t * const from_handlers[] = {
	from_fault, from_idle, from_ready, from_pushing, from_coast, from_braking
};

