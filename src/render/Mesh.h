#ifndef PROJECT_RENDER__MESH_H
#define PROJECT_RENDER__MESH_H

#include "BaseRenderable.h"
#include "MeshTriangle.h"
#include <vector>

namespace Project {
namespace Render {

class Mesh
	: public BaseRenderable {
private:

	std::vector< MeshTriangle* > triangles;
	//std::vector< MeshVertex* > vertices;

public:

	Mesh();
	~Mesh();
	Mesh(vector< MeshTriangle* > triangle_list);

	void renderGeometry(ShaderParamSetter setter);
};

}  // namespace Render
}  // namespace Project

#endif
