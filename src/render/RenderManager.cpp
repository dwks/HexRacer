#include "RenderManager.h"
#include "RenderableObject.h"
#include "opengl/MathWrapper.h"
#include "log/Logger.h"
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

	void RenderManager::setRenderProperties(RenderableObject* renderable) {

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

			if (properties->wantsShader()) {
				int wanted_shader_index = getShaderIndexByName(properties->getWantsShaderName());
				if (wanted_shader_index >= 0) {
					properties->setShaderIndex(wanted_shader_index);
				}
			}

			if (properties->hasShader()) {

				if (!shadersOverridden() && properties->getShaderIndex() != enabledShaderIndex) {
					//Disable the old shader and apply the new one
					shaderStack.push(properties->getShaderIndex());
					//disableShader(enabledShaderIndex);
					enableShader(properties->getShaderIndex());
				}

				if (properties->getShaderOverride())
					numShaderOverrides++;
			}
			
			if (properties->hasTexture()) {

				/*
				if (!texturesOverridden()) {
					//Apply the new textures
					textureStack.push(properties->getTexture());
				}
				*/
				
				//Bind the textures
				Texture* texture = properties->getTexture();
				//textureStack.push(texture);

				glActiveTexture(normalMapTexture);
				if (texture->hasNormalMap()) {
					glEnable(GL_TEXTURE_2D);
					glBindTexture(GL_TEXTURE_2D, texture->getNormalMap());
					setUniformInt("normal_map", normalMapTextureNum);
					glDisable(GL_TEXTURE_2D);
				}
				else {
					glDisable(GL_TEXTURE_2D);
					setUniformInt("normal_map", -1);
				}

				glActiveTexture(colorMapTexture);
				glActiveTexture(colorMapTexture);
				if (texture->hasColorMap()) {
					glEnable(GL_TEXTURE_2D);
					glBindTexture(GL_TEXTURE_2D, texture->getColorMap());
					setUniformInt("color_map", colorMapTextureNum);
				}
				else {
					glDisable(GL_TEXTURE_2D);
					setUniformInt("color_map", -1);
				}
				/*
				if (properties->getTextureOverride())
					numTextureOverrides++;
				*/

			}
			else {
				glActiveTexture(colorMapTexture);
				glDisable(GL_TEXTURE_2D);
				setUniformInt("normal_map", -1);
				setUniformInt("color_map", -1);
			}

		}

	}

	void RenderManager::revertRenderProperties(RenderableObject* renderable) {

		if (renderable->hasRenderProperties()) {

			RenderProperties* properties = renderable->getRenderProperties();
			
			if (properties->hasTransformation()) {
				glPopMatrix(); //Restore old transformation
			}

			if (false && properties->hasMaterial()) {

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
			
			/*
			if (properties->hasTexture()) {
				
				if (!texturesOverridden()) {
					textureStack.pop();
					//Do something here?
				}

				if (properties->getTextureOverride())
					numTextureOverrides--;
				
			}
			*/

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

	void RenderManager::loadShader(string name, string fragment_file, string vertex_file) {
		if (name.length() > 0) {
			Shader* new_shader = new Shader((GLchar*) fragment_file.c_str(),(GLchar*) vertex_file.c_str());
			new_shader->turnShaderOff();
			shader.push_back(new_shader);
			shaderName.push_back(name);
		}
		else {
			LOG(OPENGL, "Empty shader name provided");
		}
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
		if (shader_index == enabledShaderIndex)
			enabledShaderIndex = -1;
		shader[shader_index]->turnShaderOff();
	}

	Shader* RenderManager::getShaderByIndex(int shader_index) {
		if (shader_index < 0 || static_cast<unsigned int>(shader_index) > shader.size())
			return NULL;
		
		return shader[shader_index];
	}

	int RenderManager::getShaderIndexByName(string name) {

		for (unsigned int i = 0; i < shaderName.size(); i++) {
			if (shaderName[i] == name)
				return i;
		}
		return -1;

	}
	void RenderManager::setUniformInt(const char *name, GLint value) {
		Shader* active_shader = getShaderByIndex(enabledShaderIndex);
		if (active_shader) {

			int uniform_loc = active_shader->getUniLoc(name);
			if (uniform_loc > 0) {
				glUniform1i(uniform_loc, value);
			}

		}
	}
	void RenderManager::setUniformVector3(const char *name, Point point) {
		Shader* active_shader = getShaderByIndex(enabledShaderIndex);
		if (active_shader) {

			int uniform_loc = active_shader->getUniLoc(name);
			if (uniform_loc > 0) {
				GLfloat values [3] = {point.getX(), point.getY(), point.getZ()};
				glUniform3fv(uniform_loc, 1, values);
			}

		}
	}
	void RenderManager::setUniformVector4(const char *name, Color color) {
		Shader* active_shader = getShaderByIndex(enabledShaderIndex);
		if (active_shader) {

			int uniform_loc = active_shader->getUniLoc(name);
			if (uniform_loc > 0) {
				GLfloat values [4] = {color.redf(), color.greenf(), color.bluef(), color.alphaf()};
				glUniform4fv(uniform_loc, 1, values);
			}

		}
	}

	void RenderManager::setAttributeVector3(const char *name, Point point) {

		Shader* active_shader = getShaderByIndex(enabledShaderIndex);
		if (active_shader) {

			int attr_loc = active_shader->getAttrLoc(name);
			if (attr_loc > 0) {
				//GLfloat values [3] = {point.getX(), point.getY(), point.getZ()};
				glVertexAttrib3f(attr_loc, point.getX(), point.getY(), point.getZ());
			}

		}

	}
	void RenderManager::setAttributeVector4(const char *name, Color color) {
		Shader* active_shader = getShaderByIndex(enabledShaderIndex);
		if (active_shader) {

			int attr_loc = active_shader->getUniLoc(name);
			if (attr_loc > 0) {
				GLfloat values [4] = {color.redf(), color.greenf(), color.bluef(), color.alphaf()};
				glVertexAttrib4fv(attr_loc, values);
			}

		}
	}
	

}  // namespace Render
}  // namespace Project
