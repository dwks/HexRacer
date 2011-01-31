#include "MeshInstance.h"
#include "render/MeshLoader.h"
#include <istream>
#include <ostream>

namespace Project {
namespace Map {

	MeshInstance::MeshInstance(std::string mesh_name, Math::SimpleTransform _transformation) {
		meshName = mesh_name;
		meshGroup = Render::MeshLoader::getInstance()->getModelByName(meshName);
		type = DECOR;
		transformation = _transformation;
	}

	std::string MeshInstance::getTypeTitle(InstanceType _type) {
		switch (_type) {
			case DECOR:
				return "Decorative";
			case SOLID_STATIC:
				return "Solid Static";
			case SOLID_DYNAMIC:
				return "Solid Dynamic";
			default:
				return "";
		}
	}

	std::ostream &operator << (std::ostream &stream, const MeshInstance &instance) {
		stream << instance.getMeshName() << ' ' << instance.getTransformation() << ' '
			<< instance.getType();
		return stream;
	}
	std::istream &operator >> (std::istream &stream, MeshInstance &instance) {
		std::string name;
		Math::SimpleTransform transform;
		int type_index;

		stream >> name >> transform >> type_index;
		instance = MeshInstance(name, transform);
		instance.setType(static_cast<MeshInstance::InstanceType>(type_index));

		return stream;

	}


}  // namespace Map
}  // namespace Project
