#include "RenderProperties.h"
using namespace Project;
using namespace Math;

namespace Project {
namespace Render {

	RenderProperties::RenderProperties() {
		empty = true;
		transformation = NULL;
		//colorSet = false;
		color = NULL;
		colorOverrideChildren = false;
		material = NULL;
		materialOverrideChildren = false;
		materialTint = NULL;
		texture = NULL;
		textureOverrideChildren = false;
		shader = -1;
		shaderOverrideChildren = false;
		wantsShaderName = "";
	}




	RenderProperties::~RenderProperties() {
		delete transformation;
		delete color;
	}
	void RenderProperties::clearTransformation() {
		delete(transformation);
		transformation = NULL;
		changed();
	}

	void RenderProperties::setTransformation(const Math::Matrix& _transform) {
		if (!transformation)
			transformation = new Math::Matrix(_transform);
		else
			*transformation = Math::Matrix(_transform);
		changed();
	}
	void RenderProperties::clearColor() {
		delete(color);
		color = NULL;
		changed();
	}
	void RenderProperties::setColor(const OpenGL::Color& _color) {
		if (!color)
			color = new OpenGL::Color(_color);
		else
			*color = OpenGL::Color(_color);
		changed();
	}
	bool RenderProperties::wantsShader() const {
		return (wantsShaderName.length() > 0);
	}
	std::string RenderProperties::getWantsShaderName() {
		return wantsShaderName;
	}
	void RenderProperties::setWantsShaderName(std::string shader_name) {
		wantsShaderName = shader_name;
		setShaderIndex(-1);
		changed();
	}
	bool RenderProperties::hasShaderParams() const {
		return (!shaderParams.empty());
	}
	std::vector<Shader::ShaderParameter*>& RenderProperties::getShaderParams() {
		return shaderParams;
	}
	void RenderProperties::setShaderParams(std::vector<Shader::ShaderParameter*> params) {
		shaderParams = params;
		changed();
	}

	void RenderProperties::addShaderParameter(Shader::ShaderParameter* param) {
		shaderParams.push_back(param);
		changed();
	}

	void RenderProperties::changed() {

		empty = !(
			hasTransformation() ||
			hasColor() || getColorOverride() ||
			hasMaterial() || getMaterialOverride() ||
			hasMaterialTint() ||
			hasTexturePack() || getTexturePackOverride() ||
			hasShaderIndex() || getShaderOverride() ||
			wantsShader() || hasShaderParams() );

	}
}  // namespace Render
}  // namespace Project
