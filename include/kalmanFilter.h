#ifndef _Kalman_h
#define _Kalman_h

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct {
  int q; //process noise covariance
  int r; //measurement noise covariance
  int x; //value
  int p; //estimation error covariance
  int k; //kalman gain
} kalman_state;


kalman_state kalman_init(int q, int r, int p, int intial_value);
int kalman_update(kalman_state* state, int measurement);
void kalmanFunc(void);


#endif
