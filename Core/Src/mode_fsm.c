#include "mode_fsm.h"

static const servo_mode_t transition_table[SERVO_MODE_COUNT][EVT_COUNT] = {
		[SERVO_MODE_MANUAL] = {
				SERVO_MODE_AUTO_CENTER,		// EVT_SW_MODE
				SERVO_MODE_MANUAL,			// EVT_SW_CENTER
				SERVO_MODE_MANUAL			// EVT_SW_ARM
		},
		[SERVO_MODE_AUTO_CENTER] = {
				SERVO_MODE_MANUAL,
				SERVO_MODE_AUTO_MIN,
				SERVO_MODE_AUTO_CENTER
		},
		[SERVO_MODE_AUTO_MIN] = {
				SERVO_MODE_MANUAL,
				SERVO_MODE_AUTO_MAX,
				SERVO_MODE_AUTO_MIN
		},
		[SERVO_MODE_AUTO_MAX] = {
				SERVO_MODE_MANUAL,
				SERVO_MODE_AUTO_SWEEP,
				SERVO_MODE_AUTO_MAX
		},
		[SERVO_MODE_AUTO_SWEEP] = {
				SERVO_MODE_MANUAL,
				SERVO_MODE_AUTO_CENTER,
				SERVO_MODE_AUTO_SWEEP
		}
};
static servo_mode_t servo_mode = SERVO_MODE_MANUAL;
static bool armed = false;

void fsm_init(void) {
	servo_mode = SERVO_MODE_MANUAL;
	armed = false;
}

void fsm_handle_event(fsm_event_t event) {

	// look for new mode based on current servo_mode and event trigger
	servo_mode_t new_mode = transition_table[servo_mode][event];

	// change mode if new_mode is different than servo_mode (current) and disarm
	if (new_mode != servo_mode) {
		servo_mode = new_mode;
		armed = false;
	}
	// if SW_ARM button was pressed - toggle arm status
	if (event == EVT_SW_ARM) {
		armed = !armed;
	}
}

servo_mode_t fsm_get_mode(void) {
	return servo_mode;
}

bool fsm_is_armed(void) {
	return armed;
}
