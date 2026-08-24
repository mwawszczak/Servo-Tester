#ifndef INC_SERVO_H_
#define INC_SERVO_H_
#include <stdint.h>
#include <stdbool.h>

typedef enum {
	UP,
	DOWN
}dir_t;

typedef struct {
	uint32_t time_ms;
	int current_position;
	dir_t direction;
	bool target_position_reached;
}servo_state_t;

typedef enum servo_error {
	NO_ERROR,
	POSITION_ERROR
}servo_error_t;

void servo_init(void);
void servo_min(void);
void servo_max(void);
void servo_center(void);
void servo_sweep(void);
servo_error_t servo_position_set(uint16_t servo_position);
void servo_arm(void);
void servo_disarm(void);

#endif /* INC_SERVO_H_ */
