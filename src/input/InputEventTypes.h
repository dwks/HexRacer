#ifndef PROJECT_INPUT__INPUT_EVENT_TYPES_H
#define PROJECT_INPUT__INPUT_EVENT_TYPES_H

namespace Project {
namespace Input {

enum DigitalInputEvent {
	INPUT_D_PAINT,
	INPUT_D_ERASE,
	INPUT_D_WARP,
	INPUT_D_JUMP,
	INPUT_D_PHYSICS_DEBUG,
	INPUT_D_PATH_DEBUG,
	INPUT_D_CAMERA_DEBUG,
	INPUT_D_CAMERA_SWITCH_FOCUS,
	INPUT_D_CAMERA_SWITCH_FOCUS_INV,
	INPUT_D_RELOAD_CONFIG,
	INPUT_D_TOGGLE_PAUSED,
	INPUT_D_MENU_UP,
	INPUT_D_MENU_DOWN,
	INPUT_D_MENU_LEFT,
	INPUT_D_MENU_RIGHT,
	INPUT_D_MENU_CONFIRM,
	INPUT_D_MENU_BACK,
	NUM_DIGITAL_TYPES
};

enum AnalogInputEvent {
	INPUT_A_TURN,
	INPUT_A_ACCELERATE,
	INPUT_A_CAMERA_X_SPEED,
	INPUT_A_CAMERA_Z_SPEED,
	NUM_ANALOG_TYPES
};

}  // namespace Input
}  // namespace Project

#endif
