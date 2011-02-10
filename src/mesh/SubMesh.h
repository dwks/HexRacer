#ifndef PROJECT_RENDER__MESH_H
#define PROJECT_RENDER__MESH_H

#include "render/BaseRenderable.h"
#include "MeshTriangle.h"
#include "MeshTriangleFan.h"
#include "math/BSPTree3D.h"
#include "opengl/OpenGL.h"
#include "math/SpatialObjectOperator.h"
#include <vector>

namespace Project {
namespace Mesh {

/** A triangle-mesh
*/
class SubMesh
	: public Render::BaseRenderable, public Math::SpatialObjectOperator {
private:

	std::vector< Math::Triangle3D > triangles;
	std::vector< MeshTriangleFan* > triangleFans;
	Math::BSPTree3D* triangleFanTree;
	OpenGL::Material* material;
	GLuint displayList;
	Render::ShaderParamSetter* paramSetter;

	void generateTriangleFanTree();
	void generateDisplayList();

	inline void drawTriangleFan(MeshTriangleFan* fan, Render::ShaderParamSetter& setter);

	static const int TREE_SPLIT_SIZE = 20;
	static const Math::SpatialContainer::QueryType CULLING_QUERY_TYPE = Math::SpatialContainer::NEARBY;
	static const Math::BSPTree3D::SplitMethod TREE_SPLIT_METHOD = Math::BSPTree3D::MIN_OVERLAP;

public:

	SubMesh();
	~SubMesh();
	SubMesh(vector< MeshTriangle* > _triangles, OpenGL::Material* _material = NULL, bool cullable = false);

	void renderGeometry(Render::ShaderParamSetter& setter, const  Math::BoundingObject* bounding_object = NULL);
	const vector<Project::Math::Triangle3D>& getTriangles() { return triangles; }
	void operateOnObject(Math::ObjectSpatial* object);
};

}  // namespace Mesh
}  // namespace Project

#endif
