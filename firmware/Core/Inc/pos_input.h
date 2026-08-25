#ifndef INC_POS_INPUT_H_
#define INC_POS_INPUT_H_
#include <stdint.h>
#include "adc.h"

void position_input_init(void);
uint16_t position_input_get(void);


#endif /* INC_POS_INPUT_H_ */
