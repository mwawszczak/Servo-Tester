#include "buttons.h"

GPIO_PinState buttons_poll(void) {

	static uint32_t time_ms = 0;
	static GPIO_PinState btn = GPIO_PIN_RESET;
	GPIO_PinState btn_raw = HAL_GPIO_ReadPin(SW_MODE_GPIO_Port, SW_MODE_Pin);

	if (btn == btn_raw) time_ms = HAL_GetTick();
	else {
		if (HAL_GetTick() - time_ms > 3) btn = btn_raw;
	}
	return btn;
}

GPIO_PinState button_pressed(GPIO_PinState current_state) {
	static GPIO_PinState last_state = GPIO_PIN_RESET;
	GPIO_PinState result = GPIO_PIN_RESET;
	if ((last_state == GPIO_PIN_RESET) && (current_state == GPIO_PIN_SET)) {
		result = current_state;
	}

	last_state = current_state;
	// result is RESET by default, no need to reset it in else condition
	return result;
}
