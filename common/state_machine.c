#include "state_machine.h"
#include <stdlib.h>

void initialize_state_machine(state_t *handle, STATE_NAME initial_state,
							int num_states, state_transition_t * const *to_states,
							state_transition_t * const *from_states) {
	int i;

	handle->prev_state = initial_state;
	handle->curr_state = initial_state;
	handle->next_state = initial_state;

	handle->to_state_table = malloc(sizeof(state_transition_t *) * num_states);
	handle->from_state_table = malloc(sizeof(state_transition_t *) * num_states);

	for (i = 0; i < num_states; i++) {
		handle->to_state_table[i] = to_states[i];
		handle->from_state_table[i] = from_states[i];
	}
}

void state_machine_handler(state_t *handle) {
	if (handle->change_state) {

		/* call state exit function */
		handle->from_state_table[handle->curr_state](handle->flags);

		/* call state entrance function */
		handle->to_state_table[handle->next_state](handle->flags);

		/* update state information */
		handle->prev_state = handle->curr_state;
		handle->curr_state = handle->next_state;
		handle->change_state = false;

		/* TODO: always clear flags? */
		//handle->flags = 0;
	}
}

const char *state_strings[] = {
	"FAULT", "IDLE", "READY", "PUSHING", "COAST", "BRAKING"
};

