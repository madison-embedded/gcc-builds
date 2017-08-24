#ifndef _BADGERLOOP__H
#define _BADGERLOOP__H

#include <stdint.h>
#include "lwip/def.h"

/* IP addresses */
#define SPACEX_IP		1
#define DASHBOARD_IP	10
#define DEV_IP			112

/* Packet sizes */
#define SPACEXP_SIZ		34
#define DASHBOARDP_SIZ	59

/* Dashboard query parameters */
#define OUTGOING_QUERY	1
#define DASH_RESPONSE	2
#define QUERY_TO		7000
#define QUERY_RETRY		1000

/* Constant fields & utilities  */
#define TEAM_ID			0x3
#define DAQ_INT			500
#define TELEM_INT		1000
#define CHECK_THRESHOLD(get_macro, upper, lower)	\
	(get_macro > upper || get_macro < lower)

#define SOC_INITIAL		1013760
#define CM_PER_STRIP	3048

/*****************************************************************************/
/*                          Error checking parameters                        */
/*****************************************************************************/
#define VBATT_UPPER			14500	/* mV			*/
#define VBATT_LOWER			10500	/* mV			*/
#define IBATT_UPPER			13000	/* mA			*/
#define IBATT_LOWER			0		/* mA			*/
#define TBATT_UPPER			400		/* 1/10 deg C	*/
#define TBATT_LOWER			150		/* 1/10 deg C	*/

#define BRAKING_ON_P_UPPER	150		/* PSI			*/
#define BRAKING_ON_P_LOWER	100		/* PSI			*/
#define BRAKING_OFF_P_UPPER	40		/* PSI			*/
#define BRAKING_OFF_P_LOWER	0		/* PSI			*/
#define PROP_P_UPPER		4000	/* PSI			*/
#define PROP_P_LOWER		2000	/* PSI			*/

#define ACCEL_UPPER_IDLE	100		/* cm/s^2	*/
#define ACCEL_LOWER_IDLE	-100	/* cm/s^2	*/

#define TARGET_DECEL		-981
#define TARGET_END_POS		125000

#define BRAKING_COUNT_THRS	5
/*****************************************************************************/
/*****************************************************************************/

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
extern const char *fault_message;
extern int bad_value;

extern uint8_t *status;
#define SET_STATUS(val)	\
	*status = (uint8_t) val

extern int *acceleration, *velocity, *position;
#define SET_ACCEL(val)	*acceleration = htonl(val)
#define GET_ACCEL		((int) ntohl(*acceleration))
#define SET_VEL(val)	*velocity = htonl(val)
#define GET_VEL			((int) ntohl(*velocity))
#define SET_POS(val)	*position = htonl(val)
#define GET_POS			((int) ntohl(*position))
extern int *v_batt, *i_batt;
#define SET_VBATT(val)	*v_batt = htonl(val)
#define GET_VBATT		((int) ntohl(*v_batt))
#define SET_IBATT(val)	*i_batt = htonl(val)
#define GET_IBATT		((int) ntohl(*i_batt))
extern int *t_batt, *t_pod;
#define SET_TBATT(val)	*t_batt = htonl(val)
#define GET_TBATT		((int) ntohl(*t_batt))
#define SET_TPOD(val)	*t_pod = htonl(val)
#define GET_TPOD		((int) ntohl(*t_pod))
extern uint32_t *s_count;
#define SET_SCOUNT(val)	*s_count = htonl(val)
#define GET_SCOUNT		ntohl(*s_count)

extern uint16_t *p_amb;
#define SET_PAMP(val)	*p_amb = htons(val)
#define GET_PAMP		ntohs(*p_amb)
extern uint16_t *pr_p1, *pr_p2, *br_p1, *br_p2, *br_p3;
#define SET_PRP1(val)	*pr_p1 = htons(val)
#define GET_PRP1		ntohs(*pr_p1)
#define SET_PRP2(val)	*pr_p2 = htons(val)
#define GET_PRP2		ntohs(*pr_p2)
#define SET_BRP1(val)	*br_p1 = htons(val)
#define GET_BRP1		ntohs(*br_p1)
#define SET_BRP2(val)	*br_p2 = htons(val)
#define GET_BRP2		ntohs(*br_p2)
#define SET_BRP3(val)	*br_p3 = htons(val)
#define GET_BRP3		ntohs(*br_p3)

extern uint8_t *lim_states;
#define PLIM1 0x1
#define SET_PLIM1	*lim_states |= PLIM1
#define CLR_PLIM1	*lim_states &= ~PLIM1
#define GET_PLIM1	(*lim_states & PLIM1)
#define PLIM2 0x2
#define SET_PLIM2	*lim_states |= PLIM2
#define CLR_PLIM2	*lim_states &= ~PLIM2
#define GET_PLIM2	(*lim_states & PLIM2)
#define BLIM1 0x4
#define SET_BLIM1	*lim_states |= BLIM1
#define CLR_BLIM1	*lim_states &= ~BLIM1
#define GET_BLIM1	(*lim_states & BLIM1)
#define BLIM2 0x8
#define SET_BLIM2	*lim_states |= BLIM2
#define CLR_BLIM2	*lim_states &= ~BLIM2
#define GET_BLIM2	(*lim_states & BLIM2)
#define DLIM 0x10
#define SET_DLIM	*lim_states |= DLIM
#define CLR_DLIM	*lim_states &= ~DLIM
#define GET_DLIM	(*lim_states & DLIM)
extern uint32_t plim1_ts, plim2_ts, blim1_ts, blim2_ts, dlim_ts;

extern int *stopping_distance;
#define SET_STOPD(val)	*stopping_distance = htonl(val)
#define GET_STOPD		((int) ntohl(*stopping_distance))

#define SET_CHARGE_PERC(val)	*percentage = htonl(val)
#define SET_TIME_REMAINING(val)	*time_remaining = htonl(val)

/* Exposed functions */
int badgerloop_init(void);
int send_telemetry_to_SpaceX(void);
int send_telemetry_to_Dashboard(void);
int send_message_to_Dashboard(char *buf, int length);
void badgerloop_update_data(void);
int query_Dashboard(void);

/* Actuation functions */
void primary_brakes(int intensity);
void secondary_brakes(int intensity);
void vent_primary_brakes(bool open);
void vent_secondary_brakes(bool open);
void thrusters(bool on);
void vent_thrusters(bool open);

void change_state(STATE_NAME state);

#endif

