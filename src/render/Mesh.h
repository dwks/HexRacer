#ifndef PROJECT_RENDER__MESH_H
#define PROJECT_RENDER__MESH_H

#include "BaseRenderable.h"
#include "MeshTriangle.h"
#include "MeshTriangleFan.h"
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

	std::vector< Math::Triangle3D > triangles;
	std::vector< MeshTriangleFan* > triangleFans;
	Math::BSPTree3D* triangleFanTree;
	Material* material;
	GLuint displayList;

	void generateTriangleFanTree();
	void generateDisplayList();

	

	inline void drawTriangle(MeshTriangle* triangle, ShaderParamSetter& setter);

	inline void drawTriangleFan(MeshTriangleFan* fan, ShaderParamSetter& setter);

	static const int TREE_SPLIT_SIZE = 20;
	static const Math::SpatialContainer::QueryType CULLING_QUERY_TYPE = Math::SpatialContainer::NEARBY;
	static const Math::BSPTree3D::SplitMethod TREE_SPLIT_METHOD = Math::BSPTree3D::MIN_OVERLAP;

public:

	Mesh();
	~Mesh();
	Mesh(vector< MeshTriangle* > _triangles, Material* _material = NULL, bool cullable = false);

	void renderGeometry(ShaderParamSetter& setter, const  Math::BoundingObject* bounding_object = NULL);
	const vector<Project::Math::Triangle3D>& getTriangles() { return triangles; }
};

}  // namespace Render
}  // namespace Project

#endif
