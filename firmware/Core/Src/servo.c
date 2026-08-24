#include <servo.h>
#include <stdlib.h>
#include "tim.h"


static const uint16_t servo_position_center = 1500;
static const uint16_t servo_position_min = 1000;
static const uint16_t servo_position_max = 2000;
static const uint16_t step = 20;
static const uint16_t time_per_step = 20;



void servo_init(void) {

}

void servo_center(void) {
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, servo_position_center);
}

servo_error_t servo_position(uint16_t servo_position) {
	static uint32_t time_ms = 0;
	static uint16_t srv_pos_cur = 1500;
	int diff;
	if ((servo_position < servo_position_min) || (servo_position > servo_position_max))
		return POSITION_ERROR;

	// ease in/out
	if ((HAL_GetTick() - time_ms) > time_per_step) {
		time_ms = HAL_GetTick();
		diff = srv_pos_cur - servo_position;
		if (diff > 0) {
			if (diff > step) srv_pos_cur -= step;
			else srv_pos_cur -= diff;
		}
		else {
			if (abs(diff) > step )srv_pos_cur += step;
			else srv_pos_cur += abs(diff);
		}
	}
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, srv_pos_cur);
	return NO_ERROR;
}

void servo_min(void) {
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, servo_position_min);
}

void servo_max(void) {
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, servo_position_max);
}

void servo_sweep(void) {
	static uint32_t time_ms = 0;
	static uint16_t srv_pos = 1000;
	static dir_t dir = DIR_UP;

	if ((HAL_GetTick() - time_ms) > time_per_step) {
		time_ms = HAL_GetTick();

		if (dir ==  DIR_UP) {
			srv_pos += step;
			if (srv_pos >= servo_position_max) dir = DIR_DOWN;
		}
		else {
			srv_pos -= step;
			if (srv_pos <= servo_position_min) dir = DIR_UP;
		}
	}
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, srv_pos);
}

void servo_arm(void){
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
}

void servo_disarm(void) {
	HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_1);
}


