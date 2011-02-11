#include "Light.h"
#include "opengl/OpenGL.h"
#include "math/Values.h"
#include <ostream>
#include <istream>
using namespace Project;
using namespace OpenGL;
using namespace Math;

namespace Project {
namespace OpenGL {

	Light::Light(Project::Math::Point position)
		: Vertex3D(position) {

		diffuse = Color(Color::WHITE);
		specular = Color(Color::WHITE);
		ambient = Color(Color::BLACK);
		strength = 1.0f;
		hasAttenuation = true;

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

		values[0] = getPosition().getX();
		values[1] = getPosition().getY();
		values[2] = getPosition().getZ();
		values[3] = 1.0f;
		glLightfv(light, GL_POSITION, values);

		glLightf(light, GL_QUADRATIC_ATTENUATION, quadAttenuation);
		glLightf(light, GL_LINEAR_ATTENUATION, 0.0f);
		glLightf(light, GL_CONSTANT_ATTENUATION, getConstantAttenuation());
	}

	void Light::setStrength(float _strength) {
		strength = maximum(1.0f, _strength);
		updateAttenuation();
	}

	void Light::setHasAttenuation(bool has_attenuation) {
		hasAttenuation = has_attenuation;
		updateAttenuation();
	}

	void Light::updateAttenuation() {
		if (hasAttenuation)
			quadAttenuation = 1.0/strength;
		else
			quadAttenuation = 0.0;
	}

	float Light::getConstantAttenuation() const {
		if (hasAttenuation)
			return 0.75f;
		else
			return 1.0f;
	}

	std::ostream &operator << (std::ostream &stream, const Light &light) {

		stream << light.getPosition() << ' '
			<< light.getDiffuse() << ' '
			<< light.getSpecular() << ' '
			<< light.getAmbient() << ' '
			<< light.getHasAttenuation() << ' '
			<< light.getStrength();

		return stream;
	}

	std::istream &operator >> (std::istream &stream, Light &light) {

		Point position;
		Color diffuse;
		Color specular;
		Color ambient;
		bool has_attenuation;
		float strength;

		stream >> position >> diffuse  >> specular
			 >> ambient >> has_attenuation >> strength;

		light.moveCentroid(position);
		light.setDiffuse(diffuse);
		light.setSpecular(specular);
		light.setAmbient(ambient);
		light.setHasAttenuation(has_attenuation);
		light.setStrength(strength);

		return stream;
	}


}  // namespace Render
}  // namespace Project
