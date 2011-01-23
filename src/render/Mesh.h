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
	void drawTriangle(MeshTriangle* triangle, ShaderParamSetter& setter);

	static const int TREE_SPLIT_SIZE = 20;
	static const unsigned int MIN_TREE_SIZE = 300;
	static const Math::SpatialContainer::QueryType CULLING_QUERY_TYPE = Math::SpatialContainer::NEARBY;
	static const Math::BSPTree3D::SplitMethod TREE_SPLIT_METHOD = Math::BSPTree3D::MIN_OVERLAP;

public:

	Mesh();
	~Mesh();
	Mesh(vector< MeshTriangle* > _triangles, Material* _material = NULL);

	void renderGeometry(ShaderParamSetter& setter, const  Math::BoundingObject* bounding_object = NULL);
	vector<Project::Math::Triangle3D> getTriangles();
};

}  // namespace Render
}  // namespace Project

#endif
