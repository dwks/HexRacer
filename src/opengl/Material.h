#ifndef PROJECT_RENDER__MATERIAL_H
#define PROJECT_RENDER__MATERIAL_H

#include "opengl/Color.h"
#include "opengl/OpenGL.h"
#include <string>

namespace Project {
namespace OpenGL {

/** Represents a renderable material
*/
class Material {
private:

	std::string name;
	Project::OpenGL::Color diffuse;
	Project::OpenGL::Color specular;
	Project::OpenGL::Color ambient;
	float shininess;
	
public:

	Material(std::string _name = "");

	std::string getName() const;

	Project::OpenGL::Color getDiffuse() const {return diffuse;}
	Project::OpenGL::Color getSpecular() const {return specular;}
	Project::OpenGL::Color getAmbient() const {return ambient;}
	float getShininess() const {return shininess;}

	void setDiffuse(Project::OpenGL::Color color) { diffuse = color; }
	void setSpecular(Project::OpenGL::Color color) { specular = color; }
	void setAmbient(Project::OpenGL::Color color) { ambient = color; }
	void setShininess(float amount) { shininess = amount; }

	/** Apply this material to the specified polygon face @a face
	*/
	void glApply(GLenum face = GL_FRONT_AND_BACK);

	Material operator * (const Material &other) const;
};

}  // namespace Render
}  // namespace Project

#endif
