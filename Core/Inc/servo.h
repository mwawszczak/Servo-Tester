#ifndef INC_SERVO_H_
#define INC_SERVO_H_
#include <stdint.h>

typedef enum {
	DIR_UP,
	DIR_DOWN
}dir_t;

typedef enum {
	SERVO_POSITION_MIN,
	SERVO_POSITION_MAX,
	SERVO_POSITION_CENTER,
	SERVO_POSITION_MANUAL,
	SEVO_POSITION_COUNT
}servo_position_t;

typedef enum servo_error {
	NO_ERROR,
	POSITION_ERROR
}servo_error_t;

void servo_init(void);
void servo_min(void);
void servo_max(void);
void servo_center(void);
servo_error_t servo_positon(uint16_t servo_position);
void servo_arm(void);
void servo_disarm(void);

#endif /* INC_SERVO_H_ */
