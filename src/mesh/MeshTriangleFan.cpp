#include "MeshTriangleFan.h"

namespace Project {
namespace Mesh {

	MeshTriangleFan::MeshTriangleFan(std::vector<MeshVertex*> _vertices) {
		vertices = _vertices;
		if (!vertices.empty()) {
			setToObject(*vertices[0]);
			for (unsigned int i = 1; i < vertices.size(); i++) {
				expandToInclude(*vertices[i]);
			}
		}
	}

}  // namespace Mesh
}  // namespace Project
