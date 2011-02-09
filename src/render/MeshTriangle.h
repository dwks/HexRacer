#ifndef PROJECT_RENDER__MESH_TRIANGLE_H
#define PROJECT_RENDER__MESH_TRIANGLE_H

#include "math/Triangle3D.h"
#include "MeshVertex.h"

namespace Project {
namespace Render {

/** A triangle that is linked to mesh vertices
*/
class MeshTriangle
	: public Project::Math::Triangle3D {
private:
	MeshVertex* vertex [3];
public:

	MeshTriangle(MeshVertex* v1, MeshVertex* v2, MeshVertex* v3);
	~MeshTriangle();

	Project::Math::Point getVertex(short vert_index) const;
	MeshVertex* getMeshVertex(short vert_index) const;

	short getVertexIndex(const MeshVertex* vert) const;
	bool hasVertex(const MeshVertex* vert) const;
	MeshVertex* getNextFanVertex(const MeshVertex* fan1, const MeshVertex* fan2);
};

}  // namespace Render
}  // namespace Project

#endif
