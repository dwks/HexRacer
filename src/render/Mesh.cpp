#include "Mesh.h"
#include "opengl/MathWrapper.h"
using namespace std;
using namespace Project;
using namespace OpenGL;

namespace Project {
namespace Render {

	Mesh::Mesh() {}

	Mesh::~Mesh() {}

	Mesh::Mesh(vector< MeshTriangle* > triangle_list) {
		triangles = triangle_list;
	}

	void Mesh::renderGeometry(ShaderParamSetter setter) {

		glBegin(GL_TRIANGLES);
		for (unsigned int i = 0; i < triangles.size(); i++) {
			for (short j = 0; j < 3; j++) {
				MeshVertex* vert = triangles[i]->getMeshVertex(j);
				MathWrapper::glNormal(vert->getNormal());
				setter.setAttributeVector3("tangent", vert->getTangent());
				setter.setAttributeVector3("bitangent", vert->getBitangent());
				glTexCoord2d(vert->getTexCoordU(), vert->getTexCoordV());
				MathWrapper::glVertex(vert->getPosition());
			}
		}
		glEnd();
	}

}  // namespace Render
}  // namespace Project
