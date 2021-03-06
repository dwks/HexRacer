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

	std::vector< MeshTriangleFan* > triangleFans;
	std::vector< MeshTriangleFan* > redrawBuffer;
	Math::BSPTree3D* triangleFanTree;
	OpenGL::Material* material;

	const Shader::ShaderParamSetter* paramSetter;

	bool useDisplayList;
	bool genDisplayList;
	GLuint displayList;
	int displayListShader;

	bool drawingCulled;

	void generateTriangleFanTree();

	inline void drawTriangleFan(MeshTriangleFan* fan, const Shader::ShaderParamSetter& setter);

	static const int TREE_SPLIT_SIZE = 20;
	static const Math::SpatialContainer::QueryType CULLING_QUERY_TYPE = Math::SpatialContainer::NEARBY;
	static const Math::BSPTree3D::SplitMethod TREE_SPLIT_METHOD = Math::BSPTree3D::MIN_OVERLAP;

public:

	SubMesh();
	~SubMesh();
	SubMesh(const vector< MeshTriangle* >& _triangles, OpenGL::Material* _material = NULL, bool cullable = false);

	void renderGeometry(const Shader::ShaderParamSetter& setter, const Math::BoundingObject* bounding_object, const Render::RenderSettings& settings);
	void appendTriangles(std::vector<Math::Triangle3D>& vec) const;
	void operateOnObject(Math::ObjectSpatial* object);
};

}  // namespace Mesh
}  // namespace Project

#endif
