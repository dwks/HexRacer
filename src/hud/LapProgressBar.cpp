#include "LapProgressBar.h"
#include "opengl/Color.h"
#include "opengl/OpenGL.h"

namespace Project {
namespace HUD {

	void LapProgressBar::render() {

		glMatrixMode(GL_PROJECTION);
		glOrtho(0.0f, 1.0f, 0.0f, 1.0f, -1.0f, 1.0f);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		
		OpenGL::Color::glColor(OpenGL::Color::WHITE, 0.25f);

		glBegin(GL_QUADS);

		glVertex2f(0.0f, 0.0f);
		glVertex2f(1.0f, 0.0f);
		glVertex2f(1.0f, 1.0f);
		glVertex2f(0.0f, 1.0f);
		
		if (progress >= 0.0f) {

			OpenGL::Color::glColor(OpenGL::Color::GREEN, 0.5f);

			glVertex2f(0.0f, 0.0f);
			glVertex2f(1.0f, 0.0f);
			glVertex2f(1.0f, progress);
			glVertex2f(0.0f, progress);

		}
		else {

			OpenGL::Color::glColor(OpenGL::Color::RED, 0.5f);

			glVertex2f(0.0f, 1.0f+progress);
			glVertex2f(1.0f, 1.0f+progress);
			glVertex2f(1.0f, 1.0f);
			glVertex2f(0.0f, 1.0f);

		}


		glEnd();

	}

}  // namespace HUD
}  // namespace Project
