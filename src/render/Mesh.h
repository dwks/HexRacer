#ifndef PROJECT_RENDER__MESH_H
#define PROJECT_RENDER__MESH_H

#include "BaseRenderable.h"
#include "MeshTriangle.h"
#include "math/BSPTree3D.h"
#include "opengl/OpenGL.h"
#include <vector>

namespace Project {
namespace Render {

/** A triangle-mesh
*/
class Mesh
	: public BaseRenderable {
private:

	std::vector< MeshTriangle* > triangles;
	Project::Math::BSPTree3D* triangleTree;
	Material* material;
	const Project::Math::BoundingObject* cullingObject;
	Project::Math::BSPTree3D::QueryType queryType;

	void generateTriangleTree();
	void drawTriangle(MeshTriangle* triangle, ShaderParamSetter setter);

public:

	Mesh();
	~Mesh();
	Mesh(vector< MeshTriangle* > _triangles, Material* _material = NULL);

	void renderGeometry(ShaderParamSetter setter);
	void setCullingObject(const Project::Math::BoundingObject* _cullingObject);
	void setCullingQueryType(Project::Math::SpatialContainer::QueryType type);
	vector<Project::Math::Triangle3D> getTriangles();
};

}  // namespace Render
}  // namespace Project

#endif
