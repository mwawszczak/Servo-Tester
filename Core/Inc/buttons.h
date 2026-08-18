#ifndef INC_BUTTONS_H_
#define INC_BUTTONS_H_

#include <stdbool.h>
#include "gpio.h"



typedef enum {
	BTN_MODE,
	BTN_CENTER,
	BTN_ARM
}button_t;

GPIO_PinState buttons_poll(void);

#endif /* INC_BUTTONS_H_ */
