#include "kalmanFilter.h"

double realMeas, correctedMeas;
kalman_state myKalman;

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

kalman_state kalman_init(double q, double r, double p, double intial_value)
{
  kalman_state result;
  result.q = q;
  result.r = r;
  result.p = p;
  result.x = intial_value;

  return result;
}


double kalman_update(kalman_state* state, double measurement)
{
  //prediction update
  
  state->p = state->p + state->q;

  //measurement update
  state->k = state->p / (state->p + state->r);
  state->x = state->x + state->k * (measurement - state->x);
  state->p = (1 - state->k) * state->p;
  return state->x;
}


//Might want to put this in a loop within the entire measuring time period
void kalmanFunc(void){

	myKalman = kalman_init(0.125,32,1023,0);

	/********Loop********************************
	realMeas = MPU_readAcceData;	//keep reading from MPU to get x axis value
	correctedMeas =  kalman_update(myKalman, realMeas);
	*********Loop*******************************/
} 

