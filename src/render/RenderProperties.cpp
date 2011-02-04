#include "RenderProperties.h"
using namespace Project;
using namespace Math;

namespace Project {
namespace Render {

	RenderProperties::RenderProperties() {
		empty = true;
		transformationSet = false;
		colorSet = false;
		colorOverrideChildren = false;
		material = NULL;
		materialOverrideChildren = false;
		texture = NULL;
		textureOverrideChildren = false;
		shader = -1;
		shaderOverrideChildren = false;
		wantsShaderName = "";
	}

	bool RenderProperties::wantsShader() const {
		return (wantsShaderName.length() > 0);
	}
	std::string RenderProperties::getWantsShaderName() {
		return wantsShaderName;
	}
	void RenderProperties::setWantsShaderName(std::string shader_name) {
		wantsShaderName = shader_name;
		empty = false;
	}
	bool RenderProperties::hasShaderParams() const {
		return (shaderParams.size() > 0);
	}
	std::vector<ShaderParameter*>& RenderProperties::getShaderParams() {
		return shaderParams;
	}
	void RenderProperties::setShaderParams(std::vector<ShaderParameter*> params) {
		shaderParams = params;
		empty = false;
	}

	void RenderProperties::addShaderParameter(ShaderParameter* param) {
		shaderParams.push_back(param);
		empty = false;
	}

}  // namespace Render
}  // namespace Project
