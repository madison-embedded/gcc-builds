#include <string.h>
#include "badgerloop.h"
#include "hal/stm32f7xx_hal.h"
#include "lwip/err.h"
#include "lwip/udp.h"
#include "ethernetif.h"
#include "cli.h"
#include "gpio.h"
#include "state_machine.h"
#include "adc.h"
#include "mpu9250.h"
#include "honeywell.h"
#include "retro.h"

#define DEBUG_IO	0

/*****************************************************************************/
/*                                 Data Buffer                               */
/*****************************************************************************/
uint8_t telemetry_buffer[DASHBOARDP_SIZ];

/* Team ID, Status */
uint8_t *team_id = &telemetry_buffer[0];
uint8_t *status = &telemetry_buffer[1];

/* Position, Velocity, Acceleration */
int16_t accelBuffer[3]; /* holds raw values */
int accelRollingBuffer[ACCEL_BUF_SIZ];
int accelCount = 0, accelSum = 0;
int *acceleration = (int *) &telemetry_buffer[2],

	*position = (int *) &telemetry_buffer[6],
	*velocity = (int *) &telemetry_buffer[10],

	/* Battery Voltage and Current*/
	*v_batt = (int *) &telemetry_buffer[14],
	*i_batt = (int *) &telemetry_buffer[18],

	/* Battery Temperature, Pod Temperature */
	*t_batt = (int *) &telemetry_buffer[22],
	*t_pod = (int *) &telemetry_buffer[26];

/* Strip Count */
uint32_t *s_count = (uint32_t *) &telemetry_buffer[30];

/*====================== Dashboard Only Fields ==============================*/

/* Ambient Pressure */
uint16_t *p_amb = (uint16_t *) &telemetry_buffer[34],

/* Pressure Sensors */
	*pr_p1 = (uint16_t *) &telemetry_buffer[36],
	*pr_p2 = (uint16_t *) &telemetry_buffer[38],
	*br_p1 = (uint16_t *) &telemetry_buffer[40],
	*br_p2 = (uint16_t *) &telemetry_buffer[42],
	*br_p3 = (uint16_t *) &telemetry_buffer[44];

/* Limit Swtich States */
uint8_t *lim_states = &telemetry_buffer[46];
uint32_t plim1_ts, plim2_ts, blim1_ts, blim2_ts, dlim_ts;

/* Stopping distance */
int *stopping_distance = (int *) &telemetry_buffer[47];

/* State of Charge */
uint32_t *percentage = (uint32_t *) &telemetry_buffer[51];
uint32_t *time_remaining = (uint32_t *) &telemetry_buffer[55];
uint32_t soc = SOC_INITIAL, curr_draw = 0; /* 12.8V * 22A * 360s */

/* Configurable */
uint32_t DONT_BRAKE_TO = 8000, MUST_BRAKE_TO = 30000, BRAKING_COUNT_THRS = 5, ACCEL_IMPULSE = 100, TARGET_END_POS = 125000, CM_PER_STRIP = 3048, INTERPOLATE = 1, THRUST_TO = 3000;
/*****************************************************************************/
/*****************************************************************************/

int calculate_stopping_distance(int velocity, int target_decel) {

	/* must be moving forward, want to move backward */
	if (velocity <= 0 || target_decel >= 0)
		return TARGET_END_POS;

	velocity *= velocity;
	return -(velocity / target_decel);

}

void battery_voltage(void) {
	uint16_t temp_adc1, temp_adc2;

	/* READING * 3.2 * 4.3 */
	temp_adc1 = analogRead(ADC3, 13);
	temp_adc2 = temp_adc1 * 3;
	temp_adc2 += temp_adc1 / 5;
	temp_adc1 = 4 * temp_adc2;
	temp_adc2 = temp_adc1 + ((3 * temp_adc2) / 10);

	SET_VBATT(temp_adc2);	/* C3:   Analog5 - Primary Battery Voltage */
}

int thermistor_scalar(uint16_t reading) {
	float retval = (float) reading;
	float To = 60.0 + 273.0;
	float Ro = 2760.0; //at 60 Celsius the resitance is 2760 ohms
	float B = 3435.0;

	/* volts */
	retval = reading * (3.3 / 1024.0);
	/* ohm */
	retval = (33000.0 / retval)  - 10000.0;
	/* 1 / retval */
	retval = (1.0 / To) + ((1.0 / B) * log(retval / Ro));
    return (int) ((1 / retval) - 273);
}

static uint16_t adc_to_mv(uint16_t reading, uint16_t offset) {
	uint16_t adc_temp = reading;
	adc_temp -= adc_temp > offset ? offset : adc_temp;
	adc_temp *= 16;
	adc_temp /= 11;
	return (adc_temp * 3) + (adc_temp / 5);
}

uint16_t braking_sensor_scalar(uint16_t reading) {
	return (adc_to_mv(reading, 107) * 75) / 1000;
}

extern void assert_fault(const char *message);


void badgerloop_forced_data(void) {

	int temp;
	uint16_t adc_temp;

	/* When not braking, accel is a "guess", when braking accel is literal */
	if (state_handle.curr_state != BRAKING)
		SET_STOPD(calculate_stopping_distance(GET_VEL, TARGET_DECEL));
	else /* todo, can we trust the accelerometer 100% of the time? */
		SET_STOPD(calculate_stopping_distance(GET_VEL, GET_ACCEL));

	/*************************************************************************/
	/*                            digital I/O                                */
	/*************************************************************************/
	/* LIM1, braking limit switch 1 */
	temp = gpio_readPin(GPIOG, 2);
	if ((temp && !GET_BLIM1) || (!temp && GET_BLIM1)) {
		blim1_ts = ticks;
#if DEBUG
		printf("braking limit 1 changed (%d)\r\n", temp);
#endif
	}
	if (temp) SET_BLIM1;
	else CLR_BLIM1;

	/* LIM2, braking limit switch 2 */
	temp = gpio_readPin(GPIOG, 3);
	if ((temp && !GET_BLIM2) || (!temp && GET_BLIM2)) {
		blim2_ts = ticks;
#if DEBUG
		printf("braking limit 2 changed (%d)\r\n", temp);
#endif
	}
	if (temp) SET_BLIM2;
	else CLR_BLIM2;

	/* GPIO1, pusher limit switch 1 */
	temp = gpio_readPin(GPIOD, 0);
	if ((temp && !GET_PLIM1) || (!temp && GET_PLIM1)) {
		plim1_ts = ticks;
#if DEBUG
		printf("pusher limit 1 changed (%d)\r\n", temp);
#endif
	}
	if (temp) SET_PLIM1;
	else CLR_PLIM1;

	/* GPIO2, pusher limit switch 2 */
	temp = gpio_readPin(GPIOD, 1);
	if ((temp && !GET_PLIM2) || (!temp && GET_PLIM2)) {
		plim2_ts = ticks;
#if DEBUG
		printf("pusher limit 2 changed (%d)\r\n", temp);
#endif
	}
	if (temp) SET_PLIM2;
	else CLR_PLIM2;

	/* GPIO3, door limit switch? */
	temp = gpio_readPin(GPIOD, 3);
	if ((temp && !GET_DLIM) || (!temp && GET_DLIM)) {
		dlim_ts = ticks;
#if DEBUG
		printf("door limit changed (%d)\r\n", temp);
#endif
	}
	if (temp) SET_DLIM;
	else CLR_DLIM;
	/*************************************************************************/
	/*************************************************************************/

	SET_STATUS(state_handle.curr_state);

	/* I2C temp/pressure sensor */
	SET_PAMP(honeywell_readPressure());
	SET_TPOD(honeywell_readTemperature() * 10);

	battery_voltage();

	/* battery current */
	adc_temp = analogRead(ADC3, 3);
	adc_temp -= adc_temp > 120 ? 120 : adc_temp;
	adc_temp = (adc_temp * 3) + (adc_temp / 5);
	SET_IBATT((adc_temp * 1000) / 37);

	/* A4:  Analog14 - Thermistor 2 */
	SET_TBATT(thermistor_scalar(analogRead(ADC3, 6)) * 10);

	if (CHECK_THRESHOLD(GET_POS, TARGET_END_POS, -1))
		state_handle.flags |= RUN_OVER;

	if (ticks - pushing_start_ts > MUST_BRAKE_TO)
		change_state(BRAKING);
}

/* data can be overidden */
uint32_t curr_accel_ts = 0, prev_accel_ts = 0;
void badgerloop_update_data(void) {

	uint16_t adc_temp;
	static uint32_t prev_scount = 0;

	/* strip count: exti */
	SET_SCOUNT(mainRetro->count);

	/* impulse capped at configurable value */
	if (mpu9250_handler(accelBuffer)) {
		prev_accel_ts = curr_accel_ts;
		curr_accel_ts = ticks;
		int diff = 0, prev = accelRollingBuffer[accelCount % ACCEL_BUF_SIZ];
		accelSum -= prev;
		diff = ((int) accelBuffer[0] - prev > 0) ? (int) (accelBuffer[0] - prev) : (int) (prev - accelBuffer[0]);
		if (diff > ACCEL_IMPULSE) {
			accelRollingBuffer[accelCount % ACCEL_BUF_SIZ] = (accelBuffer[0] - prev > 0) ? prev + ACCEL_IMPULSE : prev - ACCEL_IMPULSE;
			accelSum += (int) accelRollingBuffer[accelCount % ACCEL_BUF_SIZ];
		} else {
			accelRollingBuffer[accelCount % ACCEL_BUF_SIZ] = accelBuffer[0];
			accelSum += (int) accelBuffer[0];
		}

		accelCount++;
		SET_ACCEL(-(accelSum / ACCEL_BUF_SIZ));

		/* interpolate */
		if (INTERPOLATE && state_handle.curr_state != IDLE && state_handle.curr_state != READY && state_handle.curr_state != FAULT)
			SET_VEL(GET_VEL + (-(accelSum / ACCEL_BUF_SIZ)*((int)(curr_accel_ts - prev_accel_ts)))/1000);
	}

	if (prev_scount < mainRetro->count) {
		prev_scount = GET_SCOUNT;
		SET_VEL(getVelocity()); // exti
	} else if (((ticks - mainRetro->curr) > MAIN_INTERVAL) && GET_SCOUNT > 2) {
		__disable_irq();
		SET_VEL((1000 * CM_PER_STRIP) / (ticks - mainRetro->curr));
		SET_POS(GET_POS + (GET_VEL*DAQ_INT)/1000);
		__enable_irq();
	}

	SET_POS(CM_PER_STRIP * GET_SCOUNT);

	/* F5:   Analog6 - Pressure 1 (CN6) */
	adc_temp = analogRead(ADC3, 9);
	adc_temp = adc_to_mv(adc_temp, 107);
	adc_temp *= 5;
	adc_temp /= 4;
	adc_temp += 15; /* not an absolute sensor */
	SET_PRP1(adc_temp);

	/* C2:   Analog4 - Pressure 2 (CN5) */
	/* 24V 5k absolute pressure sensor (1-4V output) */
	adc_temp = analogRead(ADC3, 12);
	adc_temp = adc_to_mv(adc_temp, 213);
	adc_temp *= 5;
	adc_temp /= 3;
	SET_PRP2(adc_temp);

	/* F3:   Analog8 - Pressure 3 (CN5) */
	SET_BRP3(braking_sensor_scalar(analogRead(ADC3, 15)));

	/* F4:   Analog7 - Pressure 4 (CN5) */
	SET_BRP2(braking_sensor_scalar(analogRead(ADC3, 14)));

	/* F10:  Analog9 - Pressure 2 (CN6) */
	SET_BRP1(braking_sensor_scalar(analogRead(ADC3, 8)));

	/* B1:   Analog2 - Secondary Battery Voltage */
	/* F9:  Analog13 - Secondary Battery Current */

	/* C0:   Analog3 - Pressure 1 (CN5) */

	/* F6:  Analog10 - Accelerometer X */

	/* A5:  Analog15 - Thermistor 2 */
	/* F7:  Analog11 - Thermistor 3 */
	/* F8:  Analog12 - Thermistor 4 */

	badgerloop_forced_data();
}

/* Networking */
ip_addr_t to_spacex, to_dashboard;
struct udp_pcb *udp_spacex, *udp_dashboard;
struct pbuf *spacex_payload, *dashboard_payload, *message_payload;

/* Globals */
uint32_t last_telem_timestamp, last_daq_timestamp, last_batt_timestamp;
int bad_value = 0;
state_t state_handle;
const char *fault_message = "INITIAL_VAL";
static err_t lwip_error = ERR_OK;
static uint8_t badgerloop_flags = 0;

/* for accepting commands from the dashboard */
void udp_echo_recv(void *arg, struct udp_pcb *pcb, struct pbuf *p,
	const ip_addr_t *dst_ip, u16_t dst_port) {
	//printf("got a packet from %"U16_F".%"U16_F".%"U16_F".%"U16_F" on port %d\r\n",
	//	ip4_addr1_16(dst_ip), ip4_addr2_16(dst_ip),
	//	ip4_addr3_16(dst_ip), ip4_addr4_16(dst_ip), dst_port);

	/* check for query response, it's highly unfortunate
	   we check this every time but the only way would
	   be to create some kind of tick window that checks
	   for a recent query because we "spam" the output
	   and may get this twice */
	if (!strcmp((char *) p->payload, "new phone who dis")) {
		if (badgerloop_flags & OUTGOING_QUERY)
			badgerloop_flags |= DASH_RESPONSE;
		return;
	}
	printf("%s\r\n", (char *) p->payload);
	process_input((char *) p->payload);
	pbuf_free(p);
	printPrompt();
}

int badgerloop_init(void) {

#if NETWORKING
	/* don't initialize twice, only query dashboard */
	if (udp_spacex != NULL && udp_dashboard != NULL) {
		if (eth_check_link() && query_Dashboard())
			set_stdio_target(UDP);
		return 0;
	}

	/* TODO: make configurable */
	IP4_ADDR(&to_spacex, 192, 168, 0, DEV_IP);
	IP4_ADDR(&to_dashboard, 192, 168, 0, DEV_IP);

	udp_spacex = udp_new();
	udp_dashboard = udp_new();

	if (udp_spacex == NULL || udp_dashboard == NULL) return -1;

	lwip_error = udp_connect(udp_spacex, &to_spacex, 3000);
	if (lwip_error != ERR_OK) return -1;

	lwip_error = udp_connect(udp_dashboard, &to_dashboard, 3000);
	if (lwip_error != ERR_OK) return -1;

	lwip_error = udp_bind(udp_dashboard, &(gnetif.ip_addr), 3000);
	if (lwip_error != ERR_OK) return -1;

	udp_recv(udp_dashboard, udp_echo_recv, NULL);

	if (eth_check_link() && query_Dashboard())
		set_stdio_target(UDP);
#endif

	initialize_state_machine(&state_handle, IDLE,
		to_handlers, in_handlers, from_handlers,
		state_event_timestamps, state_intervals);

	memset(accelRollingBuffer, 0x00, sizeof(int) * ACCEL_BUF_SIZ);

	/* default values */
	*team_id = TEAM_ID;

	return 0;
}

int send_telemetry_to_SpaceX(void) {

	spacex_payload = pbuf_alloc(PBUF_TRANSPORT, SPACEXP_SIZ, PBUF_POOL);
	if (spacex_payload == NULL) return -1;

	__disable_irq();
	memcpy(spacex_payload->payload, telemetry_buffer, SPACEXP_SIZ);
	__enable_irq();

	last_telem_timestamp = ticks;

	lwip_error = udp_send(udp_spacex, spacex_payload);
	pbuf_free(spacex_payload);

	return (lwip_error != ERR_OK) ? -1 : 0;
}

int send_telemetry_to_Dashboard(void) {

	dashboard_payload = pbuf_alloc(PBUF_TRANSPORT, DASHBOARDP_SIZ, PBUF_POOL);
	if (dashboard_payload == NULL) return -1;

	__disable_irq();
	memcpy(dashboard_payload->payload, telemetry_buffer, DASHBOARDP_SIZ);
	__enable_irq();

	last_telem_timestamp = ticks;

	lwip_error = udp_send(udp_dashboard, dashboard_payload);
	pbuf_free(dashboard_payload);

	return (lwip_error != ERR_OK) ? -1 : 0;
}

int send_message_to_Dashboard(char *buf, int length) {

	message_payload = pbuf_alloc(PBUF_TRANSPORT, length + 5, PBUF_POOL);
	if (message_payload == NULL) return -1;

	/* indicate to dashboard this is a message */
	memcpy(message_payload->payload, "MSG: ", 5);
	memcpy(message_payload->payload + 5, buf, length);

	last_telem_timestamp = ticks;

	lwip_error = udp_send(udp_dashboard, message_payload);
	pbuf_free(message_payload);

	return (lwip_error != ERR_OK) ? -1 : 0;
}

static err_t sendQueryMessage(void) {
	err_t err;
	const char *message = "MSG: dashboard?";
	message_payload = pbuf_alloc(PBUF_TRANSPORT, strlen(message), PBUF_POOL);
	if (message_payload == NULL) return 0;
	memcpy(message_payload->payload, message, strlen(message));
	err = udp_send(udp_dashboard, message_payload);
	pbuf_free(message_payload);
	if (err != ERR_OK)
		printf("couldn't send again: %s\r\n", lwip_strerr(lwip_error));
	return err;
}

int query_Dashboard(void) {

	uint32_t query_start, last_retry_tick = 0, attempts = 0;

	query_start = ticks;
	badgerloop_flags |= OUTGOING_QUERY;

	/* blocking poll */
	do {
		lwip_loop_handler();

		/* check if response arrived */
		if (badgerloop_flags & DASH_RESPONSE) {
			badgerloop_flags &= ~(DASH_RESPONSE | OUTGOING_QUERY);
			return 1;
		}
		/* continue to send periodically */
		if (((ticks - query_start) % QUERY_RETRY == 0) && last_retry_tick != ticks) {
			last_retry_tick = ticks;
			attempts++;
			lwip_error = sendQueryMessage();
			if (lwip_error != ERR_OK)
				return 0;
		}
	} while (ticks - query_start < QUERY_TO);

	/* did not get a response */
	printf("%s: query timed out after %lu attempts\r\n", __func__, attempts);
	badgerloop_flags &= ~OUTGOING_QUERY;
	return 0;
}

bool manual_update = false, charging = false;
void application_handler(void) {

	/* do DAQ */
	if (!(ticks % DAQ_INT) && last_daq_timestamp != ticks && !manual_update) {
		last_daq_timestamp = ticks;
		badgerloop_update_data();
	} else if (manual_update)
		badgerloop_forced_data();

	/* do telemetry */
#if NETWORKING
	if (!(ticks % TELEM_INT) && last_telem_timestamp != ticks) {
		last_telem_timestamp = ticks;
		if (gnetif.flags & NETIF_FLAG_LINK_UP) {
			if (send_telemetry_to_SpaceX() || send_telemetry_to_Dashboard()) {
				state_handle.next_state = FAULT;
				fault_message = "Sending telemetry failed!";
				state_handle.change_state = true;
			}
		/* No link, enter fault */
		} else {
			state_handle.next_state = FAULT;
			fault_message = "No network link!";
			state_handle.change_state = true;
			if (state_handle.flags & POWER_ON)
				state_handle.flags |= RETRY_INIT;
		}
	}
#endif

	/* state of charge calculations */
	if (!(ticks % 1000) && last_batt_timestamp != ticks) {
		last_batt_timestamp = ticks;
		curr_draw = (GET_VBATT * GET_IBATT) / 1000000;
		if (charging) {
			if (curr_draw < ((3000 * GET_VBATT) / 1000000))
				curr_draw = 0;
			else curr_draw -= (3000 * GET_VBATT) / 1000000;
		}
		soc -= (curr_draw);
		SET_CHARGE_PERC((soc * 100) / SOC_INITIAL);
		SET_TIME_REMAINING((curr_draw == 0) ? 1012651 : soc / curr_draw);
	}

	state_machine_handler(&state_handle);

	/* check long timeout for braking */

}

/*****************************************************************************/
/*                            Actuation Functions                            */
/*****************************************************************************/
static int primary_intensity = -1, secondary_intensity = -1;

void primary_brakes(int intensity) {
#if DEBUG_IO
	printf("primary brakes %d (%s)\r\n", intensity, intensity ? "not driven" : "driven");
#endif
	if (intensity != primary_intensity) {
		gpio_writePin(GPIOD, 6, (intensity == 0) ? true : false);
		// TODO: bit field
	}
	primary_intensity = intensity;
}

void secondary_brakes(int intensity) {
#if DEBUG_IO
	printf("secondary brakes %d (%s)\r\n", intensity, intensity ? "driven" : "not driven");
#endif
	if (intensity != secondary_intensity) {
		gpio_writePin(GPIOE, 14, (intensity == 0) ? false : true);
		// TODO: bit field
	}
	secondary_intensity = intensity;
}

void vent_primary_brakes(bool open) {
#if DEBUG_IO
	printf("vent primary brakes %s\r\n", open ? "not driven" : "driven");
#endif
	gpio_writePin(GPIOB, 11, !open);
}

void vent_secondary_brakes(bool open) {
#if DEBUG_IO
	printf("vent secondary brakes %s\r\n", open ? "not driven" : "driven");
#endif
	gpio_writePin(GPIOA, 0, !open);
}

void thrusters(bool on) {
#if DEBUG_IO
	printf("thrust %s\r\n", on ? "driven" : "not driven");
#endif
	gpio_writePin(GPIOD, 4, on);
	gpio_writePin(GPIOF, 12, on);
}

void vent_thrusters(bool open) {
#if DEBUG_IO
	printf("vent thrust %s\r\n", open ? "not driven" : "driven");
#endif
	gpio_writePin(GPIOB, 10, !open);
	gpio_writePin(GPIOD, 7, !open);
}
/*****************************************************************************/
/*****************************************************************************/

