#include "RenderManager.h"
#include "RenderableObject.h"
#include "opengl/MathWrapper.h"
#include "misc/DirectoryFunctions.h"
#include "opengl/OpenGl.h"
#include "log/Logger.h"
#include <fstream>
using namespace Project;
using namespace OpenGL;
using namespace Math;
using namespace Misc;
using namespace std;

namespace Project {
namespace Render {

	RenderManager::RenderManager(const char* config_file_name) {

		paramSetter = ShaderParamSetter(this);
		numMaterialOverrides = 0;
		numShaderOverrides = 0;
		numTextureOverrides = 0;

		enabledShaderIndex =-1;

		graphicsQuality = 2;

		lightManager = new LightManager();

		LOG(OPENGL, "Renderer Initialized.");
		LOG(OPENGL, "System Video Capabilities:");
		GLint value;
		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &value);
		LOG(OPENGL, "	Max texture image units: " << value);
		glGetIntegerv(GL_MAX_LIGHTS, &value);
		LOG(OPENGL, "	Max lights: " << value);
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &value);
		LOG(OPENGL, "	Max texture size: " << value);
		glGetIntegerv(GL_MAX_VARYING_FLOATS, &value);
		LOG(OPENGL, "	Max varying floats: " << value);

		ifstream in_file(config_file_name);
		if ((bool)in_file) {
			LOG(OPENGL, "Loading Render Config File: " << config_file_name);
			while (!in_file.eof()) {
				string var_name;
				in_file >> var_name;
				if (var_name == "graphicsQuality") {
					in_file.ignore(100, '=');
					in_file >> graphicsQuality;
					LOG(OPENGL, "	Setting Graphics Quality to " << graphicsQuality);
				}
			}
		}
		in_file.close();

	}

	RenderManager::~RenderManager() {
		delete(lightManager);
	}

	void RenderManager::setRenderProperties(RenderableObject* renderable) {

		//if (renderable->hasRenderProperties()) {

		RenderProperties* properties = renderable->getRenderProperties();
		
		if (properties->hasTransformation()) {
			glPushMatrix(); //Save previous transformation
			//Apply new transformation
			MathWrapper::glMultMatrix(properties->getTransformation());
		}

		setRenderMaterial(properties);

		setRenderShader(properties);
		
		setRenderTexture(properties);

		//}

	}

	void RenderManager::setRenderMaterial(RenderProperties* properties) {
		if (properties->hasMaterial()) {

			if (!materialsOverridden()) {
				//Apply the new material if not overridden
				materialStack.push(properties->getMaterial());
				properties->getMaterial()->glApply();
			}
		}

		if (properties->getMaterialOverride())
			numMaterialOverrides++;
	}

	void RenderManager::setRenderShader(RenderProperties* properties) {

		if (!properties->hasShader() && properties->wantsShader()) {
			int wanted_shader_index = getShaderIndexByName(properties->getWantsShaderName());
			if (wanted_shader_index < 0)
				wanted_shader_index = noShaderIndex;
			properties->setShaderIndex(wanted_shader_index);
		}

		if (properties->hasShaderParams()) {
			shaderParams.push_back(properties->getShaderParams());
		}

		if (properties->hasShader()) {

			if (!shadersOverridden()) {
				shaderStack.push(properties->getShaderIndex());
				if (properties->getShaderIndex() != enabledShaderIndex) {
					//Disable the old shader and apply the new one
					disableShader(enabledShaderIndex);
					enableShader(properties->getShaderIndex());
				}
			}
		}

		if (properties->getShaderOverride())
			numShaderOverrides++;
	}

	void RenderManager::setRenderTexture(RenderProperties* properties) {

		bool hasColorMap = false;
		bool hasNormalMap = false;

		if (properties->hasTexture()) {

			if (!texturesOverridden()) {
				
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

		}

		if (properties->getTextureOverride())
			numTextureOverrides++;

		if (!hasColorMap) {
			glActiveTexture(colorMapTexture);
			glDisable(GL_TEXTURE_2D);
		}

		//Tell the shader which textures exist
		int has_tex_values[3] = {(int) hasColorMap, (int) hasNormalMap, 0};
		setUniformIntArray(SHADER_HASTEXTURE_UNIFORM_NAME, has_tex_values, 3);
		
	}

	void RenderManager::revertRenderProperties(RenderableObject* renderable) {

		//if (renderable->hasRenderProperties()) {

		RenderProperties* properties = renderable->getRenderProperties();
		
		if (properties->hasTransformation()) {
			glPopMatrix(); //Restore old transformation
		}

		revertRenderMaterial(properties);

		revertRenderShader(properties);

		//}

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
		}

		if (properties->getMaterialOverride())
			numMaterialOverrides--;
	}
	void RenderManager::revertRenderShader(RenderProperties* properties) {

		if (properties->hasShaderParams()) {
			shaderParams.pop_back();
		}

		if (properties->hasShader()) {

			if (!shadersOverridden()) {
				shaderStack.pop();
				if (shaderStack.empty())
					disableShader(enabledShaderIndex);
				else if (shaderStack.top() != enabledShaderIndex) {
					//Disable the shader and reenable the old one
					disableShader(properties->getShaderIndex());
					enableShader(shaderStack.top());
				}
			}
		}

		if (properties->getShaderOverride())
			numShaderOverrides--;
	}

	void RenderManager::revertRenderTexture(RenderProperties* properties) {
		glDisable(GL_TEXTURE_2D);
		if (properties->getTextureOverride())
			numTextureOverrides--;
	}
	ShaderParamSetter RenderManager::getShaderParamSetter() {
		return paramSetter;
	}

	void RenderManager::loadShadersFile(string filename) {

		string directory = DirectoryFunctions::extractDirectory(filename);
		LOG(OPENGL, "Loading Shader File " << filename);

		ifstream in_file;
		in_file.open(filename.c_str());
		while (!in_file.eof()) {
			string shader_type;
			in_file >> shader_type;
			if (shader_type == "all"
				|| (shader_type == "high" && graphicsQuality >= GRAPHICS_HIGH)
				|| (shader_type == "med" && graphicsQuality == GRAPHICS_MED)
				|| (shader_type == "low" && graphicsQuality <= GRAPHICS_LOW)) {
				string shader_name;
				in_file >> shader_name;
				if (shader_name.length() > 0) {
					string frag_file;
					string vert_file;
					in_file >> frag_file;
					in_file >> vert_file;
					loadShader(shader_name, string(directory).append(frag_file), string(directory).append(vert_file));
				}
			}
		}
		in_file.close();
		
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

			LOG(OPENGL, "Shader loaded: " << name);
			LOG(OPENGL, "	Fragment Shader: " << fragment_file);
			LOG(OPENGL, "	Vertex Shader: " << vertex_file);
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

	void RenderManager::enableShader(int shader_index) {
		if (shader_index < 0 || static_cast<unsigned int>(shader_index) > shader.size())
			return;
		enabledShaderIndex = shader_index;
		shader[shader_index]->turnShaderOn();
		setShaderParameters();
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

		if (name == RENDERER_NO_SHADER_NAME)
			return noShaderIndex;

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
	void RenderManager::setShaderParameters() {
		ShaderParamSetter setter = getShaderParamSetter();
		for (unsigned int i = 0; i < shaderParams.size(); i++) {
			for (unsigned int j = 0; j < shaderParams[i].size(); j++) {
				shaderParams[i][j]->setShaderParameters(setter);
			}
		}
		//Set standard parameters
		setUniformInt(SHADER_NUMLIGHTS_UNIFORM_NAME, lightManager->getActiveLights());
	}
	

}  // namespace Render
}  // namespace Project
