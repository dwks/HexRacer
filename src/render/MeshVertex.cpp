#include "MeshVertex.h"
using namespace Project;
using namespace Math;

namespace Project {
namespace Render {

	MeshVertex::MeshVertex(Point position, Point normal, Point tangent, Point bitangent,
		float texCoordU, float texCoordV)
		: Vertex3D(position) {
		this->normal = normal;
		this->tangent = tangent;
		this->bitangent = bitangent;
		this->texCoordU = texCoordU;
		this->texCoordV = texCoordV;
	}

	Point MeshVertex::getNormal() const {
		return normal;
	}
	Point MeshVertex::getTangent() const {
		return tangent;
	}
	Point MeshVertex::getBitangent() const {
		return bitangent;
	}
	float MeshVertex::getTexCoordU() const {
		return texCoordU;
	}
	float MeshVertex::getTexCoordV() const {
		return texCoordV;
	}

}  // namespace Render
}  // namespace Project