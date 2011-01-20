#include "MeshTriangle.h"
using namespace Project;
using namespace Math;

namespace Project {
namespace Render {

	MeshTriangle::MeshTriangle(MeshVertex* v1, MeshVertex* v2, MeshVertex* v3) 
		: Triangle3D(v1->getPosition(), v2->getPosition(), v3->getPosition()) {
		vertex[0] = v1;
		vertex[1] = v2;
		vertex[2] = v3;
	}

	Point MeshTriangle::getVertex(short vert_index) const {
		return vertex[vert_index]->getPosition();
	}

	MeshVertex* MeshTriangle::getMeshVertex(short vert_index) const {
		return vertex[vert_index];
	}

}  // namespace Render
}  // namespace Project
