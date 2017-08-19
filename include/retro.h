#ifndef _RETRO_H
#define _RETRO_H

#include "exti.h"

#define FRONT 	interLine[9]
#define MIDDLE 	interLine[11]
#define REAR	interLine[13]

#define  FRONTFILTERINDEX 	FRONT.count % AVERAGE_SIZE
#define  MIDDLEFILTERINDEX 	MIDDLE.count % AVERAGE_SIZE
#define  REARFILTERINDEX 	REAR.count % AVERAGE_SIZE

#define FRONT_INTERVAL      (FRONTFILTERINDEX - 1 > FRONTFILTERINDEX) ? FRONT.filter[FILTER_LEN - 1] : FRONT.filter[FRONTFILTERINDEX - 1]
#define MIDDLE_INTERVAL     (MIDDLEFILTERINDEX - 1 > MIDDLEFILTERINDEX) ? MIDDLE.filter[FILTER_LEN - 1] : MIDDLE.filter[FRONTFILTERINDEX - 1]
#define REAR_INTERVAL       (REARFILTERINDEX - 1 > REARFILTERINDEX) ? REAR.filter[FILTER_LEN - 1] : REAR.filter[FRONTFILTERINDEX - 1]


void retroInit(void);
int getVelcity(void);
#endif /* _RETR0_H*/
