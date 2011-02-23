#include "Material.h"

namespace Project {
namespace OpenGL {

	Material::Material(std::string _name) {
		name = _name;
		shininess = 1.0f;
	}

	std::string Material::getName() const {
		return name;
	}

	void Material::glApply(GLenum face) {

		GLfloat color[4];
		color[0] = diffuse.redf();
		color[1] = diffuse.greenf();
		color[2] = diffuse.bluef();
		color[3] = diffuse.alphaf();
		glMaterialfv(face, GL_DIFFUSE, color);

		color[0] = specular.redf();
		color[1] = specular.greenf();
		color[2] = specular.bluef();
		color[3] = specular.alphaf();
		glMaterialfv(face, GL_SPECULAR, color);

		color[0] = ambient.redf();
		color[1] = ambient.greenf();
		color[2] = ambient.bluef();
		color[3] = ambient.alphaf();
		glMaterialfv(face, GL_AMBIENT, color);

		glMaterialfv(face, GL_SHININESS, &shininess);

	}

	Material Material::operator * (const Material &other) const {
		Material mat;
		mat.setDiffuse(diffuse*other.getDiffuse());
		mat.setSpecular(specular*other.getSpecular());
		mat.setAmbient(ambient*other.getAmbient());
		mat.setShininess(shininess*other.getShininess());
		return mat;
	}

}  // namespace Render
}  // namespace Project
