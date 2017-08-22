#ifndef _Kalman_h
#define _Kalman_h

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "mpu9250.h"

typedef struct {
  double q; //process noise covariance
  double r; //measurement noise covariance
  double x; //value
  double p; //estimation error covariance
  double k; //kalman gain
} kalman_state;


kalman_state kalman_init(double q, double r, double p, double intial_value);
double kalman_update(kalman_state* state, double measurement);
void kalmanFunc(void);


#endif
