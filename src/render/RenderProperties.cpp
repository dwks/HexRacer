#include "RenderProperties.h"
using namespace Project;
using namespace Math;

namespace Project {
namespace Render {

	RenderProperties::RenderProperties() {
		transformationSet = false;
		material = NULL;
		materialOverrideChildren = false;
		texture = NULL;
		textureOverrideChildren = false;
		shader = -1;
		shaderOverrideChildren = false;
		wantsShaderName = "";
	}

	bool RenderProperties::hasTransformation() const {
		return (transformationSet);
	}

	void RenderProperties::clearTransformation() {
		transformationSet = false;
	}

	void RenderProperties::setTransformation(Matrix _transform) {
		transformation = _transform;
		transformationSet = true;
	}

	Matrix RenderProperties::getTransformation() const {
		return transformation;
	}

	bool RenderProperties::hasMaterial() const {
		return (material != NULL);
	}
	void RenderProperties::clearMaterial() {
		material = NULL;
	}
	void RenderProperties::setMaterial(Material* _material) {
		material = _material;
	}
	Material* RenderProperties::getMaterial() const {
		return material;
	}
	bool RenderProperties::getMaterialOverride() const {
		return materialOverrideChildren;
	}
	void RenderProperties::setMaterialOverride(bool override_children) {
		materialOverrideChildren = override_children;
	}

	bool RenderProperties::hasTexture() const {
		return (texture != NULL);
	}
	void RenderProperties::clearTexture() {
		texture = NULL;
	}
	void RenderProperties::setTexture(Texture* _texture) {
		texture = _texture;
	}
	Texture* RenderProperties::getTexture() const {
		return texture;
	}
	/*
	bool RenderProperties::getTextureOverride() const {
		return textureOverrideChildren;
	}
	void RenderProperties::setTextureOverride(bool override_children) {
		textureOverrideChildren = override_children;
	}
	*/

	bool RenderProperties::hasShader() const {
		return (shader >= 0);
	}
	void RenderProperties::clearShader() {
		shader = -1;
	}
	void RenderProperties::setShaderIndex(int _shader) {
		shader = _shader;
	}
	int RenderProperties::getShaderIndex() const {
		return shader;
	}
	bool RenderProperties::getShaderOverride() const {
		return shaderOverrideChildren;
	}
	void RenderProperties::setShaderOverride(bool override_children) {
		shaderOverrideChildren = override_children;
	}

	bool RenderProperties::wantsShader() const {
		return (wantsShaderName.length() > 0);
	}
	std::string RenderProperties::getWantsShaderName() {
		return wantsShaderName;
	}
	void RenderProperties::setWantsShaderName(std::string shader_name) {
		wantsShaderName = shader_name;
	}

}  // namespace Render
}  // namespace Project
