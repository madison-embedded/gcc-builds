#include "state_machine.h"

void initialize_state_machine(state_t *handle, STATE_NAME initial_state,
							state_transition_t * const *to_states,
							state_handler_t * const *in_states,
							state_transition_t * const *from_states,
							unsigned int *timestamp_table,
							const unsigned int *interval_table) {

	/* Set initial state */
	handle->prev_state = initial_state;
	handle->curr_state = initial_state;
	handle->next_state = initial_state;

	/* Setup table pointers */
	handle->to_state_table = to_states;
	handle->in_state_table = in_states;
	handle->from_state_table = from_states;

	/* Timestamp table for control over event frequency */
	handle->state_timestamp_table = timestamp_table;
	handle->event_interval_table = interval_table;

	/* No flags, no state change assertion */
	handle->change_state = false;
	handle->flags = 0x0;
}

void state_machine_handler(state_t *handle) {

	/* Enter state handler */
	handle->in_state_table[handle->curr_state](handle->flags);

	/* Check if we are transitioning */
	if (handle->change_state) {

		handle->change_state = false;

		/* see if any action is necessary */
		if (handle->curr_state == handle->next_state)
			return;

		/* call state exit function */
		handle->from_state_table[handle->curr_state](handle->next_state, handle->flags);

		/* call state entrance function */
		handle->to_state_table[handle->next_state](handle->curr_state, handle->flags);

		/* update state information */
		handle->prev_state = handle->curr_state;
		handle->curr_state = handle->next_state;
	}
}

const char *state_strings[] = {
	"FAULT", "IDLE", "READY", "PUSHING", "COAST", "BRAKING"
};

