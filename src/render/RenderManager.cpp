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

			setRenderMaterial(properties);

			setRenderShader(properties);
			
			setRenderTexture(properties);

		}

	}

	void RenderManager::setRenderMaterial(RenderProperties* properties) {
		if (properties->hasMaterial()) {

			if (!materialsOverridden()) {
				//Apply the new material if not overridden
				materialStack.push(properties->getMaterial());
				properties->getMaterial()->glApply();
			}

			if (properties->getMaterialOverride())
				numMaterialOverrides++;

		}
	}

	void RenderManager::setRenderShader(RenderProperties* properties) {
		if (!properties->hasShader() && properties->wantsShader()) {
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
	}

	void RenderManager::setRenderTexture(RenderProperties* properties) {

		bool hasColorMap = false;
		bool hasNormalMap = false;

		if (properties->hasTexture()) {
				
			//Bind the textures
			Texture* texture = properties->getTexture();
			//textureStack.push(texture);

			glActiveTexture(normalMapTexture);
			if (texture->hasNormalMap()) {
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, texture->getNormalMap());
				setUniformInt(SHADER_NORMALMAP_UNIFORM_NAME, normalMapTextureNum);
				glDisable(GL_TEXTURE_2D);
				hasNormalMap = true;
			}

			glActiveTexture(colorMapTexture);
			if (texture->hasColorMap()) {
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, texture->getColorMap());
				setUniformInt(SHADER_COLORMAP_UNIFORM_NAME, colorMapTextureNum);
				hasColorMap = true;
			}

		}

		if (!hasColorMap) {
			glActiveTexture(colorMapTexture);
			glDisable(GL_TEXTURE_2D);
		}

		//Tell the shader which textures exist
		int has_tex_values[3] = {(int) hasColorMap, (int) hasNormalMap, 0};
		setUniformIntArray(SHADER_HASTEXTURE_UNIFORM_NAME, has_tex_values, 3);
		
	}

	void RenderManager::revertRenderProperties(RenderableObject* renderable) {

		if (renderable->hasRenderProperties()) {

			RenderProperties* properties = renderable->getRenderProperties();
			
			if (properties->hasTransformation()) {
				glPopMatrix(); //Restore old transformation
			}

			revertRenderMaterial(properties);
	
			revertRenderShader(properties);

		}

	}

	void RenderManager::revertRenderMaterial(RenderProperties* properties) {
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
	}
	void RenderManager::revertRenderShader(RenderProperties* properties) {
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

	void RenderManager::revertRenderTexture(RenderProperties* properties) {
		glDisable(GL_TEXTURE_2D);
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
			
			vector< ShaderAttributeLocation > attributes;

			//Temporary solution to Attribute problem
			//Preload potential vertex shader attribute locations
			int loc = new_shader->getAttrLoc("tangent");
			if (loc > -1)
				attributes.push_back(ShaderAttributeLocation("tangent", loc));
			loc = new_shader->getAttrLoc("bitangent");
			if (loc > -1)
				attributes.push_back(ShaderAttributeLocation("bitangent", loc));

			shaderAttributeLocation.push_back(attributes);
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
		int loc = getShaderUniformLocation(enabledShaderIndex, name);
		if (loc >= 0)
			glUniform1i(loc, value);
	}
	void RenderManager::setUniformIntArray(const char *name, int values[], int num_values) {
		int loc = getShaderUniformLocation(enabledShaderIndex, name);
		if (loc >= 0) {
			glUniform1iv(loc, num_values, values);
		}
	}

	void RenderManager::setUniformVector3(const char *name, Point point) {
		int loc = getShaderUniformLocation(enabledShaderIndex, name);
		if (loc >= 0) {
			glUniform3f(loc, point.getX(), point.getY(), point.getZ());
		}
	}
	void RenderManager::setUniformVector4(const char *name, Color color) {
		int loc = getShaderUniformLocation(enabledShaderIndex, name);
		if (loc >= 0) {
			glUniform4f(loc, color.redf(), color.greenf(), color.bluef(), color.alphaf());
		}
	}

	void RenderManager::setAttributeVector3(const char *name, Point point) {

		int loc = getShaderAttributeLocation(enabledShaderIndex, name);
		if (loc >= 0) {
			glVertexAttrib3f(loc, point.getX(), point.getY(), point.getZ());
		}

	}
	void RenderManager::setAttributeVector4(const char *name, Color color) {
		int loc = getShaderAttributeLocation(enabledShaderIndex, name);
		if (loc >= 0) {
			glVertexAttrib4f(loc, color.redf(), color.greenf(), color.bluef(), color.alphaf());
		}
	}

	int RenderManager::getShaderAttributeLocation(int shader_index, string name) {
		if (shader_index < 0 || static_cast<unsigned int>(shader_index) > shader.size())
			return -1;
		for (unsigned int i = 0; i < shaderAttributeLocation[shader_index].size(); i++) {
			if (shaderAttributeLocation[shader_index][i].name == name) {
				return shaderAttributeLocation[shader_index][i].location;
			}
		}
		return -1;
	}
	int RenderManager::getShaderUniformLocation(int shader_index, const char *name) {
		if (shader_index < 0 || static_cast<unsigned int>(shader_index) > shader.size())
			return -1;
		return (shader[shader_index]->getUniLoc(name));
	}
	int RenderManager::boolToInt(bool b) {
		if (b)
			return 1;
		else
			return 0;
	}
	

}  // namespace Render
}  // namespace Project
