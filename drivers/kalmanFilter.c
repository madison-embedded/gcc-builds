#include "kalmanFilter.h"

int realMeas, correctedMeas;

/*****************************	
	q process noise covariance
	r measurement noise covariance(basically sensor noise)
	p estimation noise covariance
	x x axis value
	k kalman gain

One Dimension Simplified Kalman Filter 
	x = x
	p = p + q;
	k = p / (p + r);
	x = x + k * (measurement – x);
	p = (1 – k) * p;


	q = 0.125
    r = 32
    p = 1023 "large enough to narrow down"
    x = 0 
******************************/

kalman_state kalman_init(int q, int r, int p, int intial_value) {
  kalman_state result;
  result.q = q;
  result.r = r;
  result.p = p;
  result.x = intial_value;
  return result;
}


int kalman_update(kalman_state* state, int measurement) {

  //prediction update
  state->p = state->p + state->q;

  //measurement update
  state->k = state->p / (state->p + state->r);
  state->x = state->x + state->k * (measurement - state->x);
  state->p = (1 - state->k) * state->p;
  return state->x;
}

