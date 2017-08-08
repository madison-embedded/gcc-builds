#ifndef _BADGERLOOP__H
#define _BADGERLOOP__H

#include <stdint.h>

#define SPACEX_IP		1
#define DASHBOARD_IP	10
#define DEV_IP			105

#define SPACEXP_SIZ		34
#define DASHBOARDP_SIZ	47

#define TEAM_ID	0xab

extern uint32_t last_telem_timestamp;

extern uint8_t *status;
#define FAULT	0x0
#define IDLE	0x1
#define READY	0x2
#define PUSHING	0x3
#define COAST	0x4
#define BRAKING	0x5

extern int *acceleration, *velocity, *position;
#define SET_ACCEL(val)	\
	*acceleration = htonl(val)
#define SET_VEL(val)	\
	*velocity = htonl(val)
#define SET_POS(val)	\
	*position = htonl(val)
extern int *v_batt, *i_batt;
#define SET_VBATT(val)	\
	*v_batt = htonl(val)
#define SET_IBATT(val)	\
	*i_batt = htonl(val)
extern int *t_batt, *t_pod;
#define SET_TBATT(val)	\
	*t_batt = htonl(val)
#define SET_TPOD(val)	\
	*t_pod = htonl(val)
extern uint32_t *s_count;
#define SET_SCOUNT(val)	\
	*s_count = htonl(val)

extern uint16_t *p_amb;
#define SET_PAMP(val)	\
	*p_amb = htons(val)
extern uint16_t *pr_p1, *pr_p2, *br_p1, *br_p2, *br_p3;
#define SET_PRP1(val)	\
	*pr_p1 = htons(val)
#define SET_PRP2(val)	\
	*pr_p2 = htons(val)
#define SET_BRP1(val)	\
	*br_p1 = htons(val)
#define SET_BRP2(val)	\
	*br_p2 = htons(val)
#define SET_BRP3(val)	\
	*br_p3 = htons(val)

extern uint8_t *lim_states;
#define PLIM1 0x1
#define PLIM2 0x2
#define BLIM1 0x4
#define BLIM2 0x8
#define DLIM 0x10

int badgerloop_init(void);
int send_telemetry_to_SpaceX(void);
int send_telemetry_to_Dashboard(void);

#endif

