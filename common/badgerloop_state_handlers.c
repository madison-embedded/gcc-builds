#include "badgerloop.h"
#include "state_machine.h"
#include "config.h"
#include <stdio.h>

#define DEBUG	1

const unsigned int state_intervals[] = {
	2500,	/* FAULT		*/
	2500,	/* IDLE			*/
	2500,	/* READY		*/
	2500,	/* PUSHING		*/
	2500,	/* COAST		*/
	100		/* BRAKING		*/
};

void change_state(STATE_NAME state) {
	state_handle.next_state = state;
	state_handle.change_state = true;
}

void assert_fault(const char *message) {
	change_state(FAULT);
	fault_message = message;
}

/*****************************************************************************/
/*                            Actuation Functions                            */
/*****************************************************************************/
#define BRAKING_COUNT_THRS	5

void primary_brakes(int intensity) {
	static int curr_intensity = -1;
	if (intensity != curr_intensity) {
		// Do actuation
	}
	curr_intensity = intensity;
}

void secondary_brakes(int intensity) {
	static int curr_intensity = -1;
	if (intensity != curr_intensity) {
		// Do actuation
	}
	curr_intensity = intensity;
}

void vent_primary_brakes(bool open) {

}

void vent_secondary_brakes(bool open) {

}

void engage_thrusters(void) {

}

void disengage_thrusters(void) {

}

void vent_thrusters(void) {

}
/*****************************************************************************/
/*****************************************************************************/


/*****************************************************************************/
/*                              Fault Handlers                               */
/*****************************************************************************/
void to_fault(STATE_NAME from, uint32_t flags) {
	printf("executing %s from %s\r\n", __func__, state_strings[from]);
}

void in_fault(uint32_t flags) {
#if DEBUG
	print_time();
	printf("curr fault: %s\r\n", fault_message);
#endif
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

#if DEBUG
	print_time();
	printf("%s\r\n", __func__);
#endif

	/* Initial state */
	if (flags & POWER_ON) {
		primary_brakes(0);
		secondary_brakes(0);
		disengage_thrusters();
		vent_primary_brakes(false);
		vent_secondary_brakes(false);
		flags &= POWER_ON;
	}

	/* Check battery voltage */
	if (0)
		assert_fault("Low primary battery voltage\r\n");

	/* Check battery current */
	if (0)
		assert_fault("High primary battery current\r\n");

	/* Check battery temperature */
	if (0)
		assert_fault("High primary battery temperature\r\n");

	/* Check braking pressure 1 */
	if (0)
		assert_fault("Low braking pressure 1\r\n");

	/* Check braking pressure 2 */
	if (0)
		assert_fault("Low braking pressure 2\r\n");
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
#if DEBUG
	print_time();
	printf("%s\r\n", __func__);
#endif
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
#if DEBUG
	print_time();
	printf("%s\r\n", __func__);
#endif
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
	engage_thrusters();
}

void in_coast(uint32_t flags) {
#if DEBUG
	print_time();
	printf("%s\r\n", __func__);
#endif
}

void from_coast(STATE_NAME to, uint32_t flags) {
	printf("executing %s to %s\r\n", __func__, state_strings[to]);
	disengage_thrusters();
}
/*****************************************************************************/
/*****************************************************************************/


/*****************************************************************************/
/*                               Braking Handlers                            */
/*****************************************************************************/
void to_braking(STATE_NAME from, uint32_t flags) {
	printf("executing %s from %s\r\n", __func__, state_strings[from]);
	primary_brakes(100);
}

void in_braking(uint32_t flags) {

	static int primary_low_count = 0, secondary_low_count = 0;

#if DEBUG
	print_time();
	printf("%s\r\n", __func__);
#endif

	/* Check downstream primary is low, limit switches */
	if (0 && ++primary_low_count > BRAKING_COUNT_THRS)
		secondary_brakes(100);


	/* Check braking pressure 2 */
	if (0 && ++secondary_low_count > BRAKING_COUNT_THRS)
		assert_fault("CAN'T BRAKE");

	/* vary braking intensity? */

}

void from_braking(STATE_NAME to, uint32_t flags) {
	printf("executing %s to %s\r\n", __func__, state_strings[to]);
	if (to != FAULT) {
		disengage_primary_brakes();
		disengage_secondary_brakes();
	}
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

