#ifndef _BADGERLOOP__H
#define _BADGERLOOP__H

#include <stdint.h>

typedef struct req_telem {
	uint8_t team_id;
#define TEAM_ID	0xab
	uint8_t status;
#define FAULT	0x0
#define IDLE	0x1
#define READY	0x2
#define PUSHING	0x3
#define COAST	0x4
#define BRAKING	0x5
	int acceleration,
		position,
		velocity,
		v_batt, i_batt,
		t_batt, t_pod;
	uint32_t s_count;
} req_telem_t;

typedef struct d_telem {
	uint16_t p_amb;
	uint16_t lim_states;
/* bit fields, if set closed if not open */
#define PLIM1 0x1
#define PLIM2 0x2
#define BLIM1 0x4
#define BLIM2 0x8
#define DLIM 0x10
	uint16_t pr_p1, pr_p2;
	uint16_t br_p1, br_p2;
	uint16_t br_p3, br_p4;
} d_telem_t;

typedef struct badgerloop_telemetry {
	req_telem_t	req;
	d_telem_t	dash;
} badgerloop_telemetry_t;

int badgerloop_init(void);
int send_telemetry_to_SpaceX(void);
int send_telemetry_to_Dashboard(void);

extern badgerloop_telemetry_t data;

#endif

