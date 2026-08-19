#include "led.h"

static const uint32_t led_patterns[LED_PATTERNS_COUNT] = {
		[LED_ON] = 0xFFFFFFFF,
		[LED_OFF] = 0x00000000,
		[LED_SLOW_BLINK] = 0xFFFF0000,
		[LED_FAST_BLINK] = 0xF0F0F0F0,
		[LED_FAST_BLINK_x1] = 0x20000000,
		[LED_FAST_BLINK_x2] = 0x02020000
};

static led_state_t leds[LED_COUNT] = {
		[LED_AUTO] = {
				.port = LED_MODE_AUTO_GPIO_Port,
				.pin = LED_MODE_AUTO_Pin,
				.pattern = LED_OFF,
				.position = LED_POSITION_RESET},
		[LED_MANUAL] = {
				.port = LED_MODE_MANUAL_GPIO_Port,
				.pin = LED_MODE_MANUAL_Pin,
				.pattern = LED_OFF,
				.position = LED_POSITION_RESET}
};

void led_update(void) {

}
