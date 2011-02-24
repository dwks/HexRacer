#include "MeshInstance.h"
#include "mesh/MeshLoader.h"
#include <istream>
#include <ostream>

namespace Project {
namespace Map {

	MeshInstance::MeshInstance(std::string mesh_name, Math::SimpleTransform _transformation) {
		meshName = mesh_name;
		meshGroup = Mesh::MeshLoader::getInstance()->getModelByName(meshName, true);
		type = SOLID_STATIC;
		transformation = _transformation;

		diffuseTint = OpenGL::Color::WHITE;
		specularTint = OpenGL::Color::WHITE;
		ambientTint = OpenGL::Color::WHITE;
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
		
		stream << instance.getMeshName() << ' '
			<< instance.getTransformation() << ' '
			<< instance.getType() << ' ' << instance.hasTint() << ' ';

		if (instance.hasTint()) {
			stream << instance.getDiffuseTint() << ' '
				<< instance.getSpecularTint() << ' '
				<< instance.getAmbientTint() << ' ';
		}

		return stream;
	}
	std::istream &operator >> (std::istream &stream, MeshInstance &instance) {
		std::string name;
		Math::SimpleTransform transform;
		int type_index;
		bool has_tint;

		stream >> name >> transform >> type_index >> has_tint;
		instance = MeshInstance(name, transform);
		instance.setType(static_cast<MeshInstance::InstanceType>(type_index));

		if (has_tint) {
			OpenGL::Color diffuse;
			OpenGL::Color specular;
			OpenGL::Color ambient;
			stream >> diffuse >> specular >> ambient;
			instance.setDiffuseTint(diffuse);
			instance.setSpecularTint(specular);
			instance.setAmbientTint(ambient);
		}

		return stream;

	}


}  // namespace Map
}  // namespace Project
