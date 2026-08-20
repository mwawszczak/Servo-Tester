#ifndef INC_LED_H_
#define INC_LED_H_

#include "gpio.h"

#define LED_TIME_BLOCK 32 	// 32ms for shortest blink

typedef struct {
	GPIO_TypeDef *port;
	uint16_t pin;
	uint32_t pattern;
	uint32_t position;
}led_state_t;

typedef enum {
	LED_AUTO,
	LED_MANUAL,
	LED_COUNT
}led_t;

typedef enum {
	LED_ON,
	LED_OFF,
	LED_SLOW_BLINK,
	LED_FAST_BLINK,
	LED_FAST_BLINK_x1,
	LED_FAST_BLINK_x2,
	LED_PATTERNS_COUNT
}led_pattern_t;
void led_update(void);

#endif /* INC_LED_H_ */
