#include "buttons.h"

static button_state_t btns[BTN_COUNT] = {
		[BTN_MODE] = 	{.port = SW_MODE_GPIO_Port, .pin = SW_MODE_Pin},
		[BTN_CENTER] = 	{.port = SW_CENTER_GPIO_Port, .pin = SW_CENTER_Pin},
		[BTN_ARM] =		{.port = SW_ARM_GPIO_Port, .pin = SW_ARM_Pin}
	};

static bool button_poll(button_state_t *btn) {

	bool btn_raw = (HAL_GPIO_ReadPin(btn->port, btn->pin) == GPIO_PIN_RESET);

	if (btn->state == btn_raw) btn->time_ms = HAL_GetTick();
	else {
		if (HAL_GetTick() - btn->time_ms > SW_DEBOUNCE_TIME) btn->state = btn_raw;
	}
	return btn->state;
}

static bool button_pressed(button_state_t *btn) {

	bool result = false;

	if ((btn->prev_state == false) && (btn->state == true)) {
		result = btn->state;
	}

	btn->prev_state = btn->state;
	btn->pressed = result;
	// result is RESET by default, no need to reset it in else condition
	return result;
}

bool button_get(button_t btn) {

	button_poll(&btns[btn]);
	button_pressed(&btns[btn]);

	return btns[btn].pressed;
}
