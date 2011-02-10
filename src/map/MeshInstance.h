#ifndef PROJECT_MAP__MESH_INSTANCE_H
#define PROJECT_MAP__MESH_INSTANCE_H

#include "math/SimpleTransform.h"
#include "mesh/MeshGroup.h"
#include <string>

namespace Project {
namespace Map {

class MeshInstance {
public:
	static const int NUM_INSTANCE_TYPES = 3;
	enum InstanceType { DECOR, SOLID_STATIC, SOLID_DYNAMIC };
private:
	std::string meshName;
	Mesh::MeshGroup* meshGroup;
	InstanceType type;
	Math::SimpleTransform transformation;
public:
	MeshInstance(std::string mesh_name, Math::SimpleTransform _transformation);
	std::string getMeshName() const { return meshName; }
	InstanceType getType() const { return type; }
	Math::SimpleTransform getTransformation() const { return transformation; }
	Mesh::MeshGroup* getMeshGroup() const { return meshGroup; }

	void setType(InstanceType _type) { type = _type; }
	void setTransformation(Math::SimpleTransform _transformation) { transformation = _transformation; }

	static std::string getTypeTitle(InstanceType _type);
};

std::ostream &operator << (std::ostream &stream, const MeshInstance &instance);
std::istream &operator >> (std::istream &stream, MeshInstance &instance);

}  // namespace Map
}  // namespace Project

#endif
