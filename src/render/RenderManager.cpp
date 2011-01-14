#include "RenderManager.h"
#include "RenderableObject.h"
#include "opengl/MathWrapper.h"
using namespace Project;
using namespace OpenGL;
using namespace Math;

namespace Project {
namespace Render {

	RenderManager::RenderManager() {

		paramSetter = ShaderParamSetter(this);
		numMaterialOverrides = 0;
		numShaderOverrides = 0;
		numTextureOverrides = 0;

		enabledShaderIndex =-1;

	}

	RenderManager::~RenderManager() {

	}

	void RenderManager::setRenderProperties(const RenderableObject* renderable) {

		if (renderable->hasRenderProperties()) {

			RenderProperties* properties = renderable->getRenderProperties();
			
			if (properties->hasTransformation()) {
				glPushMatrix(); //Save previous transformation
				//Apply new transformation
				MathWrapper::glMultMatrix(properties->getTransformation());
			}

			if (properties->hasMaterial()) {

				if (!materialsOverridden()) {
					//Apply the new material if not overridden
					materialStack.push(properties->getMaterial());
					properties->getMaterial()->glApply();
				}

				if (properties->getMaterialOverride())
					numMaterialOverrides++;

			}

			if (properties->hasTexture()) {

				if (!texturesOverridden()) {
					//Apply the new textures
					textureStack.push(properties->getTexture());
					//Do something here?
				}

				if (properties->getTextureOverride())
					numTextureOverrides++;

			}

			if (properties->hasShader()) {

				if (!shadersOverridden()) {
					//Disable the old shader and apply the new one
					shaderStack.push(properties->getShaderIndex());
					disableShader(enabledShaderIndex);
					enableShader(properties->getShaderIndex());
				}

				if (properties->getShaderOverride())
					numShaderOverrides++;
			}

		}

	}

	void RenderManager::revertRenderProperties(const RenderableObject* renderable) {

		if (renderable->hasRenderProperties()) {

			RenderProperties* properties = renderable->getRenderProperties();
			
			if (properties->hasTransformation()) {
				glPopMatrix(); //Restore old transformation
			}

			if (properties->hasMaterial()) {

				if (!materialsOverridden()) {
					materialStack.pop();
					//Reapply the previous material
					if (!materialStack.empty()) {
						materialStack.top()->glApply();
					}
				}

				if (properties->getMaterialOverride())
					numMaterialOverrides--;

			}

			if (properties->hasTexture()) {

				if (!texturesOverridden()) {
					textureStack.pop();
					//Do something here?
				}

				if (properties->getTextureOverride())
					numTextureOverrides--;

			}

			if (properties->hasShader()) {

				if (!shadersOverridden()) {
					//Disable the shader and reenable the old one
					disableShader(properties->getShaderIndex());
					shaderStack.pop();
					if (!shaderStack.empty()) {
						enableShader(shaderStack.top());
					}
				}

				if (properties->getShaderOverride())
					numShaderOverrides--;
			}

		}

	}

	ShaderParamSetter RenderManager::getShaderParamSetter() {
		return paramSetter;
	}

	void RenderManager::loadShader(string name, string vertex_file, string fragment_file) {

	}

	bool RenderManager::materialsOverridden() {
		return (numMaterialOverrides > 0);
	}

	bool RenderManager::texturesOverridden() {
		return (numTextureOverrides > 0);
	}

	bool RenderManager::shadersOverridden() {
		return (numShaderOverrides > 0);
	}

	void RenderManager::bindTexture(Texture* texture) {

	}

	void RenderManager::enableShader(int shader_index) {
		if (shader_index < 0 || static_cast<unsigned int>(shader_index) > shader.size())
			return;
		enabledShaderIndex = shader_index;
		shader[shader_index]->turnShaderOn();
	}

	void RenderManager::disableShader(int shader_index) {
		if (shader_index < 0 || static_cast<unsigned int>(shader_index) > shader.size())
			return;

		shader[shader_index]->turnShaderOff();
	}

	Shader* RenderManager::getShaderByIndex(int shader_index) {
		if (shader_index < 0 || static_cast<unsigned int>(shader_index) > shader.size())
			return NULL;
		
		return shader[shader_index];
	}

	void RenderManager::setUniformVector3(const char *name, Point point) {
		Shader* active_shader = getShaderByIndex(enabledShaderIndex);
		if (active_shader) {

			int uniform_loc = active_shader->getUniLoc(name);
			if (uniform_loc >= 0) {
				GLfloat values [3] = {point.getX(), point.getY(), point.getZ()};
				glUniform3fv(uniform_loc, 1, values);
			}

		}
	}
	void RenderManager::setUniformVector4(const char *name, Color color) {
		Shader* active_shader = getShaderByIndex(enabledShaderIndex);
		if (active_shader) {

			int uniform_loc = active_shader->getUniLoc(name);
			if (uniform_loc >= 0) {
				GLfloat values [4] = {color.redf(), color.greenf(), color.bluef(), color.alphaf()};
				glUniform4fv(uniform_loc, 1, values);
			}

		}
	}

	void RenderManager::setAttributeVector3(const char *name, Point point) {

		Shader* active_shader = getShaderByIndex(enabledShaderIndex);
		if (active_shader) {

			int attr_loc = active_shader->getAttrLoc(name);
			if (attr_loc >= 0) {
				GLfloat values [3] = {point.getX(), point.getY(), point.getZ()};
				glVertexAttrib3fv(attr_loc, values);
			}

		}

	}
	void RenderManager::setAttributeVector4(const char *name, Color color) {
		Shader* active_shader = getShaderByIndex(enabledShaderIndex);
		if (active_shader) {

			int attr_loc = active_shader->getUniLoc(name);
			if (attr_loc >= 0) {
				GLfloat values [4] = {color.redf(), color.greenf(), color.bluef(), color.alphaf()};
				glVertexAttrib4fv(attr_loc, values);
			}

		}
	}
	

}  // namespace Render
}  // namespace Project
