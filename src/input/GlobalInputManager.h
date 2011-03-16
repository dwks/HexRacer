#ifndef PROJECT_INPUT__GLOBAL_INPUT_MANAGER_H
#define PROJECT_INPUT__GLOBAL_INPUT_MANAGER_H

#include "JoystickManager.h"
#include "InputMapper.h"

namespace Project {
namespace Input {

class GlobalInputManager {

public:
	enum PresetMapping { NO_JOYSTICK, XB360_WINDOWS };

private:
	static GlobalInputManager* instance;
	JoystickManager* joystick;
	InputMapper* inputMapper;
public:

	static GlobalInputManager *getInstance() { return instance; }

	GlobalInputManager();
	~GlobalInputManager();

	bool findJoystick();
	bool openFirstJoystick();
	void setInputMappings(PresetMapping mapping = NO_JOYSTICK);

	JoystickManager* getJoystick() { return joystick; }
	InputMapper* getInputMapper() { return inputMapper; }
};

}  // namespace Input
}  // namespace Project

#endif
