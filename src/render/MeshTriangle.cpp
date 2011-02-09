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

		vertex[0]->addAdjacentTriangle(this);
		vertex[1]->addAdjacentTriangle(this);
		vertex[2]->addAdjacentTriangle(this);
	}

	MeshTriangle::~MeshTriangle() {
		vertex[0]->removeAdjacentTriangle(this);
		vertex[1]->removeAdjacentTriangle(this);
		vertex[2]->removeAdjacentTriangle(this);
	}

	Point MeshTriangle::getVertex(short vert_index) const {
		return vertex[vert_index]->getPosition();
	}

	MeshVertex* MeshTriangle::getMeshVertex(short vert_index) const {
		return vertex[vert_index];
	}

	short MeshTriangle::getVertexIndex(const MeshVertex* vert) const {
		for (short i = 0; i < 3; i++) {
			if (vertex[i] == vert) {
				return i;
			}
		}
		return -1;
	}

	bool MeshTriangle::hasVertex(const MeshVertex* vert) const {
		return (vertex[0] == vert || vertex[1] == vert || vertex[2] == vert);
	}

	MeshVertex* MeshTriangle::getNextFanVertex(const MeshVertex* fan1, const MeshVertex* fan2) {

		if ((fan1 != NULL && !hasVertex(fan1)) || (fan2 != NULL && !hasVertex(fan2)))
			return NULL;

		if (vertex[0] != fan1 && vertex[0] != fan2)
			return vertex[0];
		if (vertex[1] != fan1 && vertex[1] != fan2)
			return vertex[1];
		if (vertex[2] != fan1 && vertex[2] != fan2)
			return vertex[2];

		return NULL;
	}

}  // namespace Render
}  // namespace Project
