#ifndef _STATE_MACHINE__H
#define _STATE_MACHINE__H

#include <stdint.h>
#include <stdbool.h>
#include "badgerloop.h" /* don't know how to not need this */

typedef void state_transition_t(STATE_NAME to_or_from, uint32_t flags);
typedef void state_handler_t(uint32_t flags);

typedef struct state {
	STATE_NAME curr_state;
	STATE_NAME prev_state;
	STATE_NAME next_state;
	volatile bool change_state;
	volatile uint32_t flags;
#define POWER_ON	0x1
	state_transition_t * const *to_state_table;
	state_handler_t * const *in_state_table;
	state_transition_t * const *from_state_table;
	unsigned int *state_timestamp_table;
	const unsigned int *event_interval_table;
} state_t;

extern const char *state_strings[];

/* State handling */
extern state_t state_handle;
extern unsigned int state_event_timestamps[NUM_STATES];
extern const unsigned int state_intervals[];
#define GET_TIMESTAMP(state)	\
	(state_handle.state_timestamp_table[state])
#define SET_TIMESTAMP(state)	\
	state_handle.state_timestamp_table[state] = ticks
#define GET_INTERVAL(state)	\
	(state_handle.event_interval_table[state])

extern state_transition_t * const to_handlers[];
extern state_handler_t * const in_handlers[];
extern state_transition_t * const from_handlers[];

void initialize_state_machine(state_t *handle, STATE_NAME initial_state,
							state_transition_t * const *to_states,
							state_handler_t * const *in_states,
							state_transition_t * const *from_states,
							unsigned int *timestamp_table,
							const unsigned int *interval_table);
void state_machine_handler(state_t *handle);
int check_interval(STATE_NAME state);

#endif

