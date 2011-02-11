#include "ShaderParameter.h"

namespace Project {
namespace Shader{

	ShaderParameter::ShaderParameter(const char* _name, ShaderParamType _type) {
		name = _name;
		type = _type;
	}

}  // namespace Render
}  // namespace Project
