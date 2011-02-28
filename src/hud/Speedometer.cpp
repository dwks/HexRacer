#include "Speedometer.h"
#include "math/Values.h"
#include "opengl/TextureLoading.h"

namespace Project {
namespace HUD {

	Speedometer::Speedometer() {
		maxSpeed = 1.0;
		speed = 0.0;

		startAngle = PI*1.25;
		angleRange = PI*1.5;

		alphaMaskTexture = OpenGL::TextureLoading::loadTexture2D(
			"data/hud/speedometer_alpha_mask.png",
			GL_CLAMP_TO_EDGE,
			GL_CLAMP_TO_EDGE,
			GL_LINEAR,
			GL_LINEAR,
			false);
	}

	Speedometer::~Speedometer() {
		glDeleteTextures(1, &alphaMaskTexture);
	}

	void Speedometer::render(HUDRenderer* renderer) {

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		renderer->enableAlphaMask(alphaMaskTexture);

		//Draw the background
		OpenGL::Color::glColor(OpenGL::Color::WHITE, 0.35);
		glBegin(GL_QUADS);
		glVertex2f(-1.0, -1.0);
		glVertex2f(1.0, -1.0);
		glVertex2f(1.0, 1.0);
		glVertex2f(-1.0, 1.0);
		glEnd();

		//Draw the filling polygon
		double speed_angle = startAngle - angleRange*Math::minimum(speed/maxSpeed, 1.0);

		OpenGL::Color::glColor(color, 0.75);
		glBegin(GL_POLYGON);

		glVertex2f(0.0f, 0.0f);
		for (double angle = startAngle; angle > speed_angle; angle -= 0.2)
			glVertex2f(cos(angle)*2.0, sin(angle)*2.0);
		glVertex2f(cos(speed_angle)*2.0, sin(speed_angle)*2.0);
		glVertex2f(0.0f, 0.0f);

		glEnd();


		renderer->disableAlphaMask();

	}

}  // namespace HUD
}  // namespace Project
