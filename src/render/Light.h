#ifndef PROJECT_RENDER__LIGHT_H
#define PROJECT_RENDER__LIGHT_H

#include "math/Vertex3D.h"
#include "opengl/Color.h"

namespace Project {
namespace Render {

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

	void setDiffuse(Project::OpenGL::Color _diffuse) { diffuse = _diffuse; }
	void setSpecular(Project::OpenGL::Color _specular) { diffuse = _specular; }
	void setAmbient(Project::OpenGL::Color _ambient) { diffuse = _ambient; }
	void setStrength(float _strength);
	void setHasAttenuation(bool has_attenuation);

	void glApply(GLenum light) const;

};

}  // namespace Render
}  // namespace Project

#endif
