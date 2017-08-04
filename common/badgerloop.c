#include <string.h>
#include "badgerloop.h"
#include "hal/stm32f7xx_hal.h"
#include "lwip/err.h"
#include "lwip/udp.h"

ip_addr_t to_spacex, to_dashboard;
struct udp_pcb *udp_spacex, *udp_dashboard;
struct pbuf *spacex_payload, *dashboard_payload;
volatile badgerloop_telemetry_t telem_data;
uint32_t last_telem_timestamp;
err_t lwip_error = ERR_OK;

int badgerloop_init(void) {

	assert_param(sizeof(req_telem_t) == 34);
	assert_param(sizeof(badgerloop_telemetry_t) == 50);

	IP4_ADDR(&to_spacex, 192, 168, 0, 1);
	IP4_ADDR(&to_dashboard, 192, 168, 0, 10);

	udp_spacex = udp_new();
	udp_dashboard = udp_new();

	spacex_payload = pbuf_alloc(PBUF_TRANSPORT, 34, PBUF_RAM);
	dashboard_payload = pbuf_alloc(PBUF_TRANSPORT, 50, PBUF_RAM);

	if (udp_spacex == NULL || udp_dashboard == NULL ||
		spacex_payload == NULL || dashboard_payload == NULL) {
		printf("%s: memory initialization failed\r\n", __func__);
		return -1;
	}

	lwip_error = udp_connect(udp_spacex, &to_spacex, 3000);
	if (lwip_error != ERR_OK) {
		printf("%s: spacex udp_connect failed\r\n", __func__);
		return -1;
	}

	lwip_error = udp_connect(udp_dashboard, &to_dashboard, 3000);
	if (lwip_error != ERR_OK) {
		printf("%s: dashboard udp_connect failed\r\n", __func__);
		return -1;
	}

	/* default values */
	telem_data.req.team_id = TEAM_ID;
	telem_data.req.status = IDLE;

	return 0;
}

int send_telemetry_to_SpaceX(void) {

	memcpy(spacex_payload->payload, (void *) &telem_data, 34);
	last_telem_timestamp = ticks;

	lwip_error = udp_send(udp_spacex, spacex_payload);
	if (lwip_error != ERR_OK) {
		printf("spacex udp_send failed\r\n");
		return -1;
	}

	return 0;
}

int send_telemetry_to_Dashboard(void) {

	memcpy(dashboard_payload->payload, (void *) &telem_data, 50);
	last_telem_timestamp = ticks;

	lwip_error = udp_send(udp_dashboard, dashboard_payload);
	if (lwip_error != ERR_OK) {
		printf("dashboard udp_send failed\r\n");
		return -1;
	}

	return 0;
}

