#ifndef PROJECT_HUD__SPEEDOMETER_H
#define PROJECT_HUD__SPEEDOMETER_H

#include "HUDRenderer.h"
#include "opengl/Color.h"

namespace Project {
namespace HUD {

class Speedometer {
private:

	GLuint alphaMaskTexture;

	double startAngle;
	double angleRange;

	double speed;
	double maxSpeed;
	OpenGL::Color color;

public:

	Speedometer();
	~Speedometer();

	void setColor(OpenGL::Color _color) { color = _color; }
	void setMaxSpeed(double max_speed) { maxSpeed = max_speed; }
	void setSpeed(double _speed) { speed = _speed; }

	void render(HUDRenderer* renderer);
};

}  // namespace HUD
}  // namespace Project

#endif
