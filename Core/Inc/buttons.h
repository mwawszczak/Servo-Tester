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
	BTN_ARM
}button_t;

bool button_poll(button_state_t *btn);
bool button_pressed(button_state_t *btn);
#endif /* INC_BUTTONS_H_ */
