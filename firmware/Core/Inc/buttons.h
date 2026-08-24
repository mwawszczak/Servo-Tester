#ifndef INC_BUTTONS_H_
#define INC_BUTTONS_H_

#define SW_DEBOUNCE_TIME 3
#include <stdbool.h>
#include "gpio.h"

typedef struct {
	GPIO_TypeDef *port;
	uint16_t pin;
	uint32_t time_ms;
	bool state;		// state after debounce
	bool prev_state;	// previous button state for edge detection
	bool pressed;
} button_state_t;

typedef enum {
	BTN_MODE,
	BTN_CENTER,
	BTN_ARM,
	BTN_COUNT
}button_t;

bool button_get(button_t btn);
#endif /* INC_BUTTONS_H_ */
