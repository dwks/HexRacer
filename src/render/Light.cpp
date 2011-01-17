#include "Light.h"
#include "opengl/OpenGL.h"
#include "math/Values.h"
using namespace Project;
using namespace OpenGL;
using namespace Math;

namespace Project {
namespace Render {

	Light::Light(Project::Math::Point position)
		: Vertex3D(position) {

		diffuse = Color(Color::WHITE);
		specular = Color(Color::WHITE);
		ambient = Color(Color::BLACK);
		strength = 1.0f;

	}

	void Light::glApply(GLenum light) const {

		glEnable(light);

		GLfloat values[4] = {diffuse.redf(), diffuse.greenf(), diffuse.bluef(), diffuse.alphaf()};
		glLightfv(light, GL_DIFFUSE, values);

		values[0] = specular.redf();
		values[1] = specular.greenf();
		values[2] = specular.bluef();
		values[3] = specular.alphaf();
		glLightfv(light, GL_SPECULAR, values);

		values[0] = ambient.redf();
		values[1] = ambient.greenf();
		values[2] = ambient.bluef();
		values[3] = ambient.alphaf();
		glLightfv(light, GL_AMBIENT, values);

		values[0] = ambient.redf();
		values[1] = ambient.greenf();
		values[2] = ambient.bluef();
		values[3] = ambient.alphaf();
		glLightfv(light, GL_AMBIENT, values);

		values[0] = getPosition().getX();
		values[1] = getPosition().getY();
		values[2] = getPosition().getZ();
		values[3] = 1.0f;
		glLightfv(light, GL_POSITION, values);

		glLightf(light, GL_QUADRATIC_ATTENUATION, 1.0f/strength);
	}

	void Light::setStrength(float _strength) {
		strength = maximum(1.0f, _strength);
	}

}  // namespace Render
}  // namespace Project
