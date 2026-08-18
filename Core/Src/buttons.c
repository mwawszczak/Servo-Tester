#include "buttons.h"

bool button_poll(button_state_t *btn) {

	bool btn_raw = (HAL_GPIO_ReadPin(btn->port, btn->pin) == GPIO_PIN_RESET);

	if (btn->state == btn_raw) btn->time_ms = HAL_GetTick();
	else {
		if (HAL_GetTick() - btn->time_ms > SW_DEBOUNCE_TIME) btn->state = btn_raw;
	}
	return btn->state;
}

bool button_pressed(button_state_t *btn) {

	bool result = false;

	if ((btn->prev_state == false) && (btn->state == true)) {
		result = btn->state;
	}

	btn->prev_state = btn->state;
	btn->pressed = result;
	// result is RESET by default, no need to reset it in else condition
	return result;
}
