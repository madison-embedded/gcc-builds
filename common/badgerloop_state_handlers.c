#include "badgerloop.h"
#include "state_machine.h"
#include "ethernetif.h"
#include "config.h"
#include <stdio.h>

const unsigned int state_intervals[] = {
	250,	/* FAULT		*/
	100,	/* IDLE			*/
	250,	/* READY		*/
	250,	/* PUSHING		*/
	250,	/* COAST		*/
	100		/* BRAKING		*/
};

void change_state(STATE_NAME state) {
	state_handle.next_state = state;
	state_handle.change_state = true;
}

void assert_fault(const char *message) {
	change_state(FAULT);
	printf("%s: %s\r\n", __func__, message);
	fault_message = message;
}

void clear_flag(uint32_t flags) {
	state_handle.flags &= ~flags;
}

void set_flag(uint32_t flags) {
	state_handle.flags |= flags;
}

bool verify_DAQ(void) {
	/* Check battery voltage */
	if (CHECK_THRESHOLD(GET_VBATT, VBATT_UPPER, VBATT_LOWER)) {
		assert_fault("primary battery voltage OOR\r\n");
		bad_value = GET_VBATT;
		return false;
	}

	/* Check battery current */
	if (CHECK_THRESHOLD(GET_IBATT, IBATT_UPPER, IBATT_LOWER)) {
		assert_fault("primary battery current OOR\r\n");
		bad_value = GET_IBATT;
		return false;
	}

	/* Check battery temperature */
	if (CHECK_THRESHOLD(GET_TBATT, TBATT_UPPER, TBATT_LOWER)) {
		assert_fault("primary battery temperature OOR\r\n");
		bad_value = GET_TBATT;
		return false;
	}

	/* Check braking pressure 1, upstream? */
	if (CHECK_THRESHOLD(GET_BRP1, BRAKING_ON_P_UPPER, BRAKING_ON_P_LOWER)) {
		assert_fault("Low braking pressure 1\r\n");
		bad_value = GET_BRP1;
		return false;
	}

	/* Check braking pressure 2, upstream? */
	if (CHECK_THRESHOLD(GET_BRP2, BRAKING_ON_P_UPPER, BRAKING_ON_P_LOWER)) {
		assert_fault("Low braking pressure 2\r\n");
		bad_value = GET_BRP2;
		return false;
	}

	/* Check braking pressure 3, downstream? */
	if (CHECK_THRESHOLD(GET_BRP3, BRAKING_OFF_P_UPPER, BRAKING_OFF_P_LOWER)) {
		assert_fault("High braking pressure 3\r\n");
		bad_value = GET_BRP3;
		return false;
	}

	/* Check accelerometer */
	if (CHECK_THRESHOLD(GET_ACCEL, ACCEL_UPPER_IDLE, ACCEL_LOWER_IDLE)) {
		assert_fault("Accelerating\r\n");
		bad_value = GET_ACCEL;
		return false;
	}

	/* Check velocity */
	if (CHECK_THRESHOLD(GET_VEL, 5, -5)) {
		assert_fault("Velocity not zero\r\n");
		bad_value = GET_VEL;
		return false;
	}

	/* Check position */
	if (CHECK_THRESHOLD(GET_POS, 0, 0)) {
		assert_fault("Position not zero\r\n");
		bad_value = GET_POS;
		return false;
	}

	return true;
}


/*****************************************************************************/
/*                              Fault Handlers                               */
/*****************************************************************************/
void to_fault(STATE_NAME from, uint32_t flags) {

	/* Lord have mercy */
	if (from == BRAKING) {
		if (ticks - pushing_start_ts > DONT_BRAKE_TO) {
			primary_brakes(100);
			secondary_brakes(100);
		} else change_state(BRAKING);
	}

	else if (from == PUSHING)
		change_state(COAST);
	else if (from == COAST)
		change_state(BRAKING);
	else if (from == READY)
		change_state(IDLE);

}

void in_fault(uint32_t flags) {

	// TODO: determine if fault means the run is
	//       over if so, set RUN_OVER flag and
	//       go to idle so we can vent everything

	if ((flags & RETRY_INIT) && gnetif.flags & NETIF_FLAG_LINK_UP) {
		change_state(IDLE);
		clear_flag(RETRY_INIT);
	}

	if (flags & RUN_OVER)
		change_state(IDLE);
}

void from_fault(STATE_NAME to, uint32_t flags) {

}
/*****************************************************************************/
/*****************************************************************************/


/*****************************************************************************/
/*                                 Idle Handlers                             */
/*****************************************************************************/
void to_idle(STATE_NAME from, uint32_t flags) {

}

void in_idle(uint32_t flags) {

	/* Initial state */
	if (flags & POWER_ON) {
		primary_brakes(0); /* don't drive, save power */
		secondary_brakes(0);
		thrusters(0);
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

}
/*****************************************************************************/
/*****************************************************************************/


/*****************************************************************************/
/*                             Ready Handlers                                */
/*****************************************************************************/
void to_ready(STATE_NAME from, uint32_t flags) {
		//vent_primary_brakes(false);
		//primary_brakes(0);
}

void in_ready(uint32_t flags) {

	/* Push phase condition: either pusher limit switch */
	if (!(GET_PLIM1 && GET_PLIM2) && (GET_ACCEL < ACCEL_IMPULSE)) // || (GET_SCOUNT > 0)
		change_state(PUSHING);

}

void from_ready(STATE_NAME to, uint32_t flags) {

}
/*****************************************************************************/
/*****************************************************************************/


/*****************************************************************************/
/*                               Pushing Handlers                            */
/*****************************************************************************/
uint32_t pushing_start_ts = 0;

void to_pushing(STATE_NAME from, uint32_t flags) {
	pushing_start_ts = ticks;
}


void in_pushing(uint32_t flags) {

	/* Coast phase condition: limit switches + delay */
	if (GET_PLIM1 && GET_PLIM2)
		if (ticks - ((plim1_ts > plim2_ts) ? plim1_ts : plim2_ts) > 1000)
			change_state(COAST);

	if (ticks - pushing_start_ts > MUST_BRAKE_TO)
		change_state(BRAKING);

}

void from_pushing(STATE_NAME to, uint32_t flags) {

}
/*****************************************************************************/
/*****************************************************************************/


/*****************************************************************************/
/*                                 Coast Handlers                            */
/*****************************************************************************/
void to_coast(STATE_NAME from, uint32_t flags) {
	if (from != BRAKING && from != FAULT) {
		/* Check braking pressure 1, upstream? */
		if (CHECK_THRESHOLD(GET_BRP1, BRAKING_ON_P_UPPER, BRAKING_ON_P_LOWER)) {
			assert_fault("Low braking pressure 1\r\n");
			bad_value = GET_BRP1;
			vent_thrusters(true);
		} else if (CHECK_THRESHOLD(GET_BRP2, BRAKING_ON_P_UPPER, BRAKING_ON_P_LOWER)) {
			assert_fault("Low braking pressure 2\r\n");
			bad_value = GET_BRP2;
			vent_thrusters(true);
		} else thrusters(100);
	}
	else vent_thrusters(true);
}

void in_coast(uint32_t flags) {

	/* Braking phase condition: curr_pos + stopd = target end_pos */
	if ((GET_STOPD != -1 && ((GET_STOPD + GET_POS) > TARGET_END_POS)))
		change_state(BRAKING);
	else if (GET_STOPD == -1)
		assert_fault("Unknown stopping distance");

}

void from_coast(STATE_NAME to, uint32_t flags) {

	thrusters(0);

}
/*****************************************************************************/
/*****************************************************************************/


/*****************************************************************************/
/*                               Braking Handlers                            */
/*****************************************************************************/
void to_braking(STATE_NAME from, uint32_t flags) {

	if (ticks - pushing_start_ts < DONT_BRAKE_TO)
		change_state(COAST);
	else primary_brakes(100);

}

void in_braking(uint32_t flags) {

	static int primary_low_count = 0;

	/* Check downstream primary is too low and one of limit switches is open
	   OR curr_pos + stopd > target_pos */
	if (((CHECK_THRESHOLD(GET_BRP3, BRAKING_ON_P_UPPER, BRAKING_ON_P_LOWER)
		&& (GET_BLIM1 && GET_BLIM2)) ||
		(GET_STOPD != -1 && ((GET_STOPD + GET_POS) > TARGET_END_POS)))
		&& ++primary_low_count > BRAKING_COUNT_THRS && (ticks - pushing_start_ts > DONT_BRAKE_TO)) {
		secondary_brakes(100);
	}

	if (ticks - pushing_start_ts > DONT_BRAKE_TO)
		primary_brakes(100);

	/* check stopping distance threshold for secondary */

	/* if secondary on & braking fast, turn off? */

}

void from_braking(STATE_NAME to, uint32_t flags) {

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

