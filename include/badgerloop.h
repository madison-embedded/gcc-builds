#ifndef _BADGERLOOP__H
#define _BADGERLOOP__H

#include <stdint.h>

/* IP addresses */
#define SPACEX_IP		1
#define DASHBOARD_IP	10
#define DEV_IP			104

/* Packet sizes */
#define SPACEXP_SIZ		34
#define DASHBOARDP_SIZ	47

/* Dashboard query parameters */
#define OUTGOING_QUERY	1
#define DASH_RESPONSE	2
#define QUERY_TO		7000
#define QUERY_RETRY		1000

/* Constant fields */
#define TEAM_ID	0xab

/* State definitions */
#define NUM_STATES 6
typedef enum asdf {
	FAULT = 0,
	IDLE = 1,
	READY = 2,
	PUSHING = 3,
	COAST = 4,
	BRAKING = 5
} STATE_NAME;

extern uint32_t last_telem_timestamp;

extern uint8_t *status;
#define SET_STATUS(val)	\
	*status = (uint8_t) val

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
#define SET_PLIM1	*lim_states |= PLIM1
#define CLR_PLIM1	*lim_states &= PLIM1
#define PLIM2 0x2
#define SET_PLIM2	*lim_states |= PLIM2
#define CLR_PLIM2	*lim_states &= PLIM2
#define BLIM1 0x4
#define SET_BLIM1	*lim_states |= BLIM1
#define CLR_BLIM1	*lim_states &= BLIM1
#define BLIM2 0x8
#define SET_BLIM2	*lim_states |= BLIM2
#define CLR_BLIM2	*lim_states &= BLIM2
#define DLIM 0x10
#define SET_DLIM	*lim_states |= DLIM
#define CLR_DLIM	*lim_states &= DLIM

/* Exposed functions */
int badgerloop_init(void);
int send_telemetry_to_SpaceX(void);
int send_telemetry_to_Dashboard(void);
int send_message_to_Dashboard(char *buf, int length);
void badgerloop_update_data(void);
int query_Dashboard(void);

#endif

