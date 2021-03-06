#ifndef PROJECT_RENDER__LIGHT_H
#define PROJECT_RENDER__LIGHT_H

#include <iosfwd>
#include "math/Vertex3D.h"
#include "opengl/Color.h"

namespace Project {
namespace OpenGL {

class Light
	: public Project::Math::Vertex3D {
private:

	Project::OpenGL::Color diffuse;
	Project::OpenGL::Color specular;
	Project::OpenGL::Color ambient;
	float strength;
	float quadAttenuation;
	bool hasAttenuation;

	void updateAttenuation();

public:
	
	Light(Project::Math::Point position = Project::Math::Point());

	Project::OpenGL::Color getDiffuse() const { return diffuse; }
	Project::OpenGL::Color getSpecular() const { return specular; }
	Project::OpenGL::Color getAmbient() const { return ambient; }
	float getStrength() const { return strength; }
	bool getHasAttenuation() const { return hasAttenuation; }
	float getQuadraticAttenuation() const { return quadAttenuation; }
	float getConstantAttenuation() const;

	void setDiffuse(Project::OpenGL::Color _diffuse) { diffuse = _diffuse; }
	void setSpecular(Project::OpenGL::Color _specular) { specular = _specular; }
	void setAmbient(Project::OpenGL::Color _ambient) { ambient = _ambient; }
	void setStrength(float _strength);
	void setHasAttenuation(bool has_attenuation);

	void glApply(GLenum light) const;

};

std::ostream &operator << (std::ostream &stream, const Light& light);
std::istream &operator >> (std::istream &stream, Light &light);

}  // namespace Render
}  // namespace Project

#endif
