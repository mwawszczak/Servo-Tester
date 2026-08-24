#include <servo.h>
#include <stdlib.h>
#include "tim.h"


static const uint16_t servo_position_center = 1500;
static const uint16_t servo_position_min = 1000;
static const uint16_t servo_position_max = 2000;
static const uint16_t step = 20;
static const uint16_t time_per_step = 20;

static servo_state_t servo = {
		.time_ms = 0,
		.current_position = servo_position_center,
		.direction = UP,
		.target_position_reached = false
};

servo_error_t servo_position_set(uint16_t servo_position) {

	int diff;
	if ((servo_position < servo_position_min) || (servo_position > servo_position_max))
		return POSITION_ERROR;

	// check if current_position is equal to target position
	if (servo.current_position != servo_position) {
		servo.target_position_reached = false;
		// ease in/out
		if ((HAL_GetTick() - servo.time_ms) > time_per_step) {
			servo.time_ms = HAL_GetTick();
			diff = servo.current_position - servo_position;
			if (diff > 0) {
				if (diff > step) servo.current_position -= step;
				// if current difference is less then setp -
				// use diff as last step to prevent overshooting
				else servo.current_position -= diff;
			}
			else {
				if (abs(diff) > step )servo.current_position += step;
				else servo.current_position += abs(diff);
			}
		}
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, servo.current_position);
	}
	else servo.target_position_reached = true;
	return NO_ERROR;
}

void servo_init(void) {
	// set servo in center position and populate servo variable with real values
	servo_position_set(servo_position_center);
}

void servo_center(void) {
	servo_position_set(servo_position_center);
}

void servo_min(void) {
	servo_position_set(servo_position_min);
}

void servo_max(void) {
	servo_position_set(servo_position_max);
}

void servo_sweep(void) {
		static int srv_pos = 1000;

	if (servo.target_position_reached) {
		if (servo.direction ==  UP) {
			srv_pos += step;
			if (srv_pos >= servo_position_max) servo.direction = DOWN;
		}
		else {
			srv_pos -= step;
			if (srv_pos <= servo_position_min) servo.direction = UP;
		}
	}
	servo_position_set(srv_pos);
}

void servo_arm(void){
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
}

void servo_disarm(void) {
	HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_1);
}


