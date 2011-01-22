#include "Mesh.h"
#include "opengl/MathWrapper.h"
#include "log/Logger.h"
using namespace std;
using namespace Project;
using namespace OpenGL;
using namespace Math;

namespace Project {
namespace Render {

	Mesh::Mesh() {

		triangleTree = NULL;
		cullingObject = NULL;
		material = NULL;
		queryType = SpatialContainer::BOX_INTERSECT;
	}

	Mesh::Mesh(vector< MeshTriangle* > _triangles, Material* _material) {
		triangles = _triangles;
		material = _material;

		triangleTree = NULL;
		cullingObject = NULL;
		queryType = SpatialContainer::BOX_INTERSECT;

		if (triangles.size() >= MIN_TREE_SIZE) {
			generateTriangleTree();
		}
	}

	Mesh::~Mesh() {
		for (unsigned int i = 0; i < triangles.size(); i++)
			delete(triangles[i]);
		if (material != NULL)
			delete(material);
	}

	void Mesh::generateTriangleTree() {

		BoundingBox3D bounding_box;

		vector<ObjectSpatial*> tri_obj;

		if (triangles.size() > 0) {
			bounding_box.setToObject(*triangles[0]);
			tri_obj.push_back(triangles[0]);
			for (unsigned int i = 1; i < triangles.size(); i++) {
				bounding_box.expandToInclude(*triangles[i]);
				tri_obj.push_back(triangles[i]);
			}
		}

		triangleTree = new BSPTree3D(bounding_box, TREE_SPLIT_METHOD, TREE_SPLIT_SIZE);
		triangleTree->add(tri_obj);
		LOG(OPENGL, "Generated BSP Tree with " << triangles.size() << " objects with height " << triangleTree->getHeight());

	}

	void Mesh::renderGeometry(ShaderParamSetter& setter, const BoundingObject* bounding_object) {

		glBegin(GL_TRIANGLES);

		if (bounding_object == NULL || triangleTree == NULL) {
			for (unsigned int i = 0; i < triangles.size(); i++) {
				drawTriangle(triangles[i], setter);
			}
		}
		else {
			vector<ObjectSpatial*> culled_triangles = triangleTree->query(*bounding_object, CULLING_QUERY_TYPE);
			for (unsigned int i = 0; i < culled_triangles.size(); i++) {
				drawTriangle((MeshTriangle*) culled_triangles[i], setter);
			}
		}

		glEnd();

	}

	void Mesh::drawTriangle(MeshTriangle* triangle, ShaderParamSetter& setter) {
		for (short j = 0; j < 3; j++) {
			MeshVertex* vert = triangle->getMeshVertex(j);
			MathWrapper::glNormal(vert->getNormal());
			setter.setAttributeVector3("tangent", vert->getTangent());
			setter.setAttributeVector3("bitangent", vert->getBitangent());
			glTexCoord2d(vert->getTexCoordU(), vert->getTexCoordV());
			MathWrapper::glVertex(vert->getPosition());
		}
	}


	vector<Triangle3D> Mesh::getTriangles() {
		vector<Triangle3D> return_list;
		for (unsigned int i = 0; i < triangles.size(); i++) {
			//return_list.push_back(Triangle3D(*triangles[i]));
			return_list.push_back(
				Triangle3D(
					triangles[i]->getVertex(0),
					triangles[i]->getVertex(1),
					triangles[i]->getVertex(2)
				));
		}
		return return_list;
	}

}  // namespace Render
}  // namespace Project
