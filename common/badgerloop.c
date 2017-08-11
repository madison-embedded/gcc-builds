#include <string.h>
#include "badgerloop.h"
#include "hal/stm32f7xx_hal.h"
#include "lwip/err.h"
#include "lwip/udp.h"
#include "ethernetif.h"
#include "cli.h"

/*****************************************************************************/
/*                                 Data Buffer                               */
/*****************************************************************************/
uint8_t telemetry_buffer[DASHBOARDP_SIZ];

/* Team ID, Status */
uint8_t *team_id = &telemetry_buffer[0];
uint8_t *status = &telemetry_buffer[1];

/* Position, Velocity, Acceleration */
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
/*****************************************************************************/
/*****************************************************************************/

void badgerloop_update_data(void) {

	/* combination of strips and accelerometer */
	SET_ACCEL(1);
	SET_VEL(-1);
	SET_POS(-4);

	/* I2C temp/pressure sensor */
	SET_PAMP(1);
	SET_TPOD(-2);

	/* analog voltages */
	SET_VBATT(-3);
	SET_IBATT(2);
	SET_TBATT(-5);
	SET_PRP1(1);
	SET_PRP2(1);
	SET_BRP1(1);
	SET_BRP2(1);
	SET_BRP3(1);

	/* strip count */
	SET_SCOUNT(4);

	/* digital I/O */
	if (0) SET_PLIM1;
	else CLR_PLIM1;

	if (0) SET_PLIM2;
	else CLR_PLIM2;

	if (0) SET_BLIM1;
	else CLR_BLIM1;

	if (0) SET_BLIM2;
	else CLR_BLIM2;

	if (0) SET_DLIM;
	else CLR_DLIM;
}

/* Networking */
ip_addr_t to_spacex, to_dashboard;
struct udp_pcb *udp_spacex, *udp_dashboard;
struct pbuf *spacex_payload, *dashboard_payload, *message_payload;

/* Globals */
uint32_t last_telem_timestamp;
static err_t lwip_error = ERR_OK;
static uint8_t badgerloop_flags = 0;
#define OUTGOING_QUERY	1
#define DASH_RESPONSE	2

/* for accepting commands from the dashboard */
void udp_echo_recv(void *arg, struct udp_pcb *pcb, struct pbuf *p,
	const ip_addr_t *dst_ip, u16_t dst_port) {
	//printf("got a packet from %"U16_F".%"U16_F".%"U16_F".%"U16_F" on port %d\r\n",
	//	ip4_addr1_16(dst_ip), ip4_addr2_16(dst_ip),
	//	ip4_addr3_16(dst_ip), ip4_addr4_16(dst_ip), dst_port);

	/* check for query response */
	if (badgerloop_flags & OUTGOING_QUERY) {
		if (!strcmp((char *) p->payload, "new phone who dis")) {
			badgerloop_flags |= DASH_RESPONSE;
			return;
		}
	}
	printf("%s\r\n", (char *) p->payload);
	process_input((char *) p->payload);
	pbuf_free(p);
	printPrompt();
}

int badgerloop_init(void) {

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

	/* default values */
	*team_id = TEAM_ID;
	SET_STATUS(IDLE);

	/* for endianness testing */
	badgerloop_update_data();

	return 0;
}

int send_telemetry_to_SpaceX(void) {

	spacex_payload = pbuf_alloc(PBUF_TRANSPORT, SPACEXP_SIZ, PBUF_POOL);
	if (spacex_payload == NULL) return -1;

	memcpy(spacex_payload->payload, telemetry_buffer, SPACEXP_SIZ);

	last_telem_timestamp = ticks;

	lwip_error = udp_send(udp_spacex, spacex_payload);
	pbuf_free(spacex_payload);

	return (lwip_error != ERR_OK) ? -1 : 0;
}

int send_telemetry_to_Dashboard(void) {

	dashboard_payload = pbuf_alloc(PBUF_TRANSPORT, DASHBOARDP_SIZ, PBUF_POOL);
	if (dashboard_payload == NULL) return -1;

	memcpy(dashboard_payload->payload, telemetry_buffer, DASHBOARDP_SIZ);

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

int query_Dashboard(void) {

	const char *message = "MSG: dashboard?";
	uint32_t query_start;

	message_payload = pbuf_alloc(PBUF_TRANSPORT, strlen(message), PBUF_POOL);
	if (message_payload == NULL) return 0;

	memcpy(message_payload->payload, message, strlen(message));
	query_start = ticks;
	badgerloop_flags |= OUTGOING_QUERY;

	/* blocking poll */
	do {
		lwip_loop_handler();

		/* check if response arrived */
		if (badgerloop_flags & DASH_RESPONSE) {
			pbuf_free(message_payload);
			printf("%s: got the response\r\n", __func__);
			badgerloop_flags &= ~(DASH_RESPONSE | OUTGOING_QUERY);
			return 1;
		}
		/* continue to send periodically */
		if ((ticks - query_start) % QUERY_RETRY == 0) {
			lwip_error = udp_send(udp_dashboard, message_payload);
			if (lwip_error != ERR_OK) {
				pbuf_free(message_payload);
				return 0;
			}
		}
	} while (ticks - query_start < QUERY_TO);

	/* did not get a response */
	printf("%s: query timed out\r\n", __func__);
	badgerloop_flags &= ~OUTGOING_QUERY;
	pbuf_free(message_payload);
	return 0;
}

