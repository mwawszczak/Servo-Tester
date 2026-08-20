#include "led.h"
static const uint32_t led_position_reset = 0x80000000;
static const uint32_t led_patterns[LED_PATTERNS_COUNT] = {
		[LED_ON] = 0xFFFFFFFF,
		[LED_OFF] = 0x00000000,
		[LED_SLOW_BLINK] = 0xFFFF0000,
		[LED_FAST_BLINK] = 0xAAAAAAAA,
		[LED_FAST_BLINK_x1] = 0x80000000,
		[LED_FAST_BLINK_x2] = 0xA0000000
};

static led_state_t leds[LED_COUNT] = {
		[LED_AUTO] = {
				.port = LED_MODE_AUTO_GPIO_Port,
				.pin = LED_MODE_AUTO_Pin,
				.pattern = led_patterns[LED_OFF],
				.position = led_position_reset},
		[LED_MANUAL] = {
				.port = LED_MODE_MANUAL_GPIO_Port,
				.pin = LED_MODE_MANUAL_Pin,
				.pattern = led_patterns[LED_OFF],
				.position = led_position_reset}
};
void led_on(led_state_t led) {
	HAL_GPIO_WritePin(led.port, led.pin, GPIO_PIN_SET);
}

void led_off(led_state_t led) {
	HAL_GPIO_WritePin(led.port, led.pin, GPIO_PIN_RESET);
}

void led_update(void) {
	static uint32_t time_ms = 0;
	uint32_t time_ms_now = HAL_GetTick();
	// if LED_TIME_BLOCK time passed - reset timer and update led state
	if ((time_ms_now - time_ms) > LED_TIME_BLOCK) {

		time_ms = time_ms_now;
		for (led_t led = LED_AUTO; led < LED_COUNT; led++) {
			if (leds[led].position & leds[led].pattern) led_on(leds[led]);
			else led_off(leds[led]);

			leds[led].position >>= 1;
			if (leds[led].position == 0) leds[led].position = 0x80000000;
		}
	}
}
