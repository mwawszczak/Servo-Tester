#ifndef INC_MODE_FSM_H_
#define INC_MODE_FSM_H_

#include <stdbool.h>

typedef enum {
	EVT_SW_MODE,
	EVT_SW_CENTER,
	EVT_SW_ARM,
	EVT_COUNT
} fsm_event_t;

typedef enum {
	SERVO_MODE_MANUAL,
	SERVO_MODE_AUTO_CENTER,
	SERVO_MODE_AUTO_MIN,
	SERVO_MODE_AUTO_MAX,
	SERVO_MODE_AUTO_SWEEP,
	SERVO_MODE_COUNT
} servo_mode_t;

typedef enum {
	ARMED,
	DISARMED,
	ARM_STATUS_COUNT
} arm_status_t;

void fsm_init(void);
void fsm_handle_event(fsm_event_t event);
servo_mode_t fsm_get_mode(void);
arm_status_t fsm_is_armed(void);

#endif /* INC_MODE_FSM_H_ */
