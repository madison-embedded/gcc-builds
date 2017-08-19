#include "badgerloop.h"
#include "state_machine.h"
#include "ethernetif.h"
#include "config.h"
#include <stdio.h>

#define DEBUG	0

const unsigned int state_intervals[] = {
	2500,	/* FAULT		*/
	100,	/* IDLE			*/
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

void clear_flag(uint32_t flags) {
	state_handle.flags &= flags;
}

void verify_DAQ(void) {
	/* Check battery voltage */
	if (CHECK_THRESHOLD(GET_VBATT, VBATT_UPPER, VBATT_LOWER))
		assert_fault("Low primary battery voltage\r\n");

	/* Check battery current */
	if (CHECK_THRESHOLD(GET_IBATT, IBATT_UPPER, IBATT_LOWER))
		assert_fault("High primary battery current\r\n");

	/* Check battery temperature */
	if (CHECK_THRESHOLD(GET_TBATT, TBATT_UPPER, TBATT_LOWER))
		assert_fault("High primary battery temperature\r\n");

	/* Check braking pressure 1, upstream? */
	if (CHECK_THRESHOLD(GET_BRP1, BRAKING_ON_P_UPPER, BRAKING_ON_P_LOWER))
		assert_fault("Low braking pressure 1\r\n");

	/* Check braking pressure 2, upstream? */
	if (CHECK_THRESHOLD(GET_BRP2, BRAKING_ON_P_UPPER, BRAKING_ON_P_LOWER))
		assert_fault("Low braking pressure 2\r\n");

	/* Check braking pressure 3, downstream? */
	if (CHECK_THRESHOLD(GET_BRP3, BRAKING_OFF_P_UPPER, BRAKING_OFF_P_LOWER))
		assert_fault("High braking pressure 3\r\n");

	/* Check accelerometer */
	if (CHECK_THRESHOLD(GET_ACCEL, ACCEL_UPPER_IDLE, ACCEL_LOWER_IDLE))
		assert_fault("Accelerating\r\n");

	/* Check velocity */
	if (CHECK_THRESHOLD(GET_VEL, 5, -5))
		assert_fault("Velocity not zero\r\n");

	/* Check position */
	if (CHECK_THRESHOLD(GET_POS, 0, 0))
		assert_fault("Position not zero\r\n");
}


/*****************************************************************************/
/*                            Actuation Functions                            */
/*****************************************************************************/
#define BRAKING_COUNT_THRS	5
static int primary_intensity = -1, secondary_intensity = -1;

void primary_brakes(int intensity) {
	if (intensity != primary_intensity) {
		// Do actuation
	}
	primary_intensity = intensity;
}

void secondary_brakes(int intensity) {
	if (intensity != secondary_intensity) {
		// Do actuation
	}
	secondary_intensity = intensity;
}

void vent_primary_brakes(bool open) {

}

void vent_secondary_brakes(bool open) {

}

void engage_thrusters(void) {

}

void disengage_thrusters(void) {

}

void vent_thrusters(bool open) {

}
/*****************************************************************************/
/*****************************************************************************/


/*****************************************************************************/
/*                              Fault Handlers                               */
/*****************************************************************************/
void to_fault(STATE_NAME from, uint32_t flags) {
	printf("executing %s from %s\r\n", __func__, state_strings[from]);

	/* Lord have mercy */
	if (from == BRAKING)
		secondary_brakes(100);
}

void in_fault(uint32_t flags) {
#if DEBUG
	print_time();
	printf("curr fault: %s\r\n", fault_message);
#endif
	// TODO: determine if fault == run_over
	//       if so, set RUN_OVER flag and go to
	//       idle so we can vent everything

	if ((flags & RETRY_INIT) && gnetif.flags & NETIF_FLAG_LINK_UP) {
		change_state(IDLE);
		clear_flag(RETRY_INIT);
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
		vent_thrusters(false);
		clear_flag(POWER_ON);
	}

	/* Run over */
	if (flags & RUN_OVER) {
		// TODO: check a different set of conditions?
		vent_thrusters(true);
		vent_primary_brakes(true);
		vent_secondary_brakes(true);
		printf("Run over!\r\n");
		return;
	}

	verify_DAQ();

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

	/* Push phase condition */
	if (0)
		change_state(PUSHING);

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

	/* Coast phase condition */
	if (0)
		change_state(PUSHING);
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

	/* Braking phase condition */
	if (0)
		change_state(BRAKING);
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

	static int primary_low_count = 0;

#if DEBUG
	print_time();
	printf("%s\r\n", __func__);
#endif

	/* Check downstream primary is low, limit switches, target accel */
	if (((CHECK_THRESHOLD(GET_BRP3, BRAKING_ON_P_UPPER, BRAKING_ON_P_LOWER)
		&& (!GET_BLIM1 || !GET_BLIM2)) ||
		CHECK_THRESHOLD(GET_ACCEL, ACCEL_UPPER_BRAKING, ACCEL_LOWER_BRAKING))
		&& ++primary_low_count > BRAKING_COUNT_THRS)
		secondary_brakes(100);

	/* vary braking intensity? */

	/* if secondary on & braking fast, turn off? */

}

void from_braking(STATE_NAME to, uint32_t flags) {
	printf("executing %s to %s\r\n", __func__, state_strings[to]);
	if (to != FAULT) {
		primary_brakes(0);
		secondary_brakes(0);
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

