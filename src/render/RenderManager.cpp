#include "RenderManager.h"
#include "RenderableObject.h"
#include "opengl/MathWrapper.h"
#include "misc/DirectoryFunctions.h"
#include "opengl/OpenGL.h"
#include "log/Logger.h"
#include "RenderSettings.h"
#include <fstream>
using namespace Project;
using namespace OpenGL;
using namespace Math;
using namespace Misc;
using namespace std;

namespace Project {
namespace Render {

	RenderManager::RenderManager() {

		settings = new RenderSettings();

		shaderManager = new Shader::ShaderManager(*settings);

		numMaterialOverrides = 0;
		numShaderOverrides = 0;
		numTextureOverrides = 0;

		numTransformations = 0;

		camera = NULL;
		cubeMap = NULL;

		for (int i = 0; i < NUM_STANDARD_TEXTURES; i++)
			hasTexture[i] = 0;

		lightManager = new LightManager(settings->getMaxLights());

		LOG(OPENGL, "Renderer Initialized.");

		/*
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
		*/

	}

	RenderManager::~RenderManager() {
		delete(settings);
		delete(shaderManager);
		delete(lightManager);
	}

	void RenderManager::setRenderProperties(RenderProperties* properties) {

		if (properties->isEmpty())
			return;
		
		if (properties->hasTransformation()) {
			glPushMatrix(); //Save previous transformation
			MathWrapper::glMultMatrix(properties->getTransformation());//Apply new transformation
			numTransformations++;
		}

		setRenderColor(properties);
		setRenderMaterial(properties);
		setRenderTexture(properties);
		setRenderShader(properties);
		setShaderParameters();

	}

	void RenderManager::setRenderColor(RenderProperties* properties) {
		if (properties->hasColor()) {

			if (!colorsOverridden()) {
				colorStack.push(properties->getColor());
				Color::glColor(properties->getColor());
			}
		}

		if (properties->getColorOverride())
			numColorOverrides++;
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

		if (!properties->hasShaderIndex() && properties->wantsShader())
			properties->setShaderIndex(shaderManager->shaderIndexFromName(properties->getWantsShaderName()));

		if (properties->hasShaderParams()) {
			shaderParams.push_back(properties->getShaderParams());
		}

		if (properties->hasShaderIndex()) {

			if (!shadersOverridden()) {
				shaderStack.push(properties->getShaderIndex());
				if (properties->getShaderIndex() != shaderManager->getActiveShaderIndex()) {
					shaderManager->disableShader();
					shaderManager->enableShader(properties->getShaderIndex());
				}
				//shaderManager->enableShader(properties->getShaderIndex());
				/*
				if (properties->getShaderIndex() != enabledShaderIndex) {
					//Disable the old shader and apply the new one
					disableShader(enabledShaderIndex);
					enableShader(properties->getShaderIndex());
				}
				*/
			}
		}

		if (properties->getShaderOverride())
			numShaderOverrides++;
	}

	void RenderManager::setRenderTexture(RenderProperties* properties) {

		hasTexture[COLOR_MAP_TEXTURE_INDEX] = 0;
		hasTexture[NORMAL_MAP_TEXTURE_INDEX] = 0;
		hasTexture[GLOW_MAP_TEXTURE_INDEX] = 0;

		if (properties->hasTexturePack()) {

			if (!texturesOverridden()) {
				
				//Bind the textures
				TexturePack* texture = properties->getTexturePack();

				if (texture->hasGlowMap()) {
					glActiveTexture(GLOW_MAP_TEXTURE_UNIT);
					glBindTexture(GL_TEXTURE_2D, texture->getGlowMap());
					hasTexture[GLOW_MAP_TEXTURE_INDEX] = 1;
				}

				if (texture->hasNormalMap()) {
					glActiveTexture(NORMAL_MAP_TEXTURE_UNIT);
					glBindTexture(GL_TEXTURE_2D, texture->getNormalMap());
					hasTexture[NORMAL_MAP_TEXTURE_INDEX] = 1;
				}

				glActiveTexture(COLOR_MAP_TEXTURE_UNIT);
				if (texture->hasColorMap()) {
					glEnable(GL_TEXTURE_2D);
					glBindTexture(GL_TEXTURE_2D, texture->getColorMap());
					hasTexture[COLOR_MAP_TEXTURE_INDEX] = 1;
				}

			}

		}

		if (properties->getTexturePackOverride())
			numTextureOverrides++;

		if (hasTexture[COLOR_MAP_TEXTURE_INDEX] == 0)
			glDisable(GL_TEXTURE_2D);
		
	}

	void RenderManager::revertRenderProperties(RenderProperties* properties) {

		if (properties->isEmpty())
			return;
		
		if (properties->hasTransformation()) {
			glPopMatrix(); //Restore old transformation
			numTransformations--;
		}

		revertRenderColor(properties);
		revertRenderMaterial(properties);
		revertRenderTexture(properties);
		revertRenderShader(properties);

	}

	void RenderManager::revertRenderColor(RenderProperties* properties) {
		if (properties->hasColor()) {

			if (!colorsOverridden()) {
				colorStack.pop();
				//Reapply the previous color
				if (!colorStack.empty()) {
					Color::glColor(colorStack.top());
				}
				else {
					Color::glColor(Color::WHITE);
				}
			}
		}

		if (properties->getColorOverride())
			numColorOverrides--;
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

		if (properties->hasShaderIndex()) {

			if (!shadersOverridden()) {
				shaderStack.pop();
				if (shaderStack.empty())
					shaderManager->disableShader();
				else if (shaderStack.top() != shaderManager->getActiveShaderIndex()) {
					shaderManager->disableShader();
					shaderManager->enableShader(shaderStack.top());
				}
			}
		}

		if (properties->getShaderOverride())
			numShaderOverrides--;
	}

	void RenderManager::revertRenderTexture(RenderProperties* properties) {
		glActiveTexture(COLOR_MAP_TEXTURE_UNIT);
		glDisable(GL_TEXTURE_2D);
		if (properties->getTexturePackOverride())
			numTextureOverrides--;
	}

	/*
	void RenderManager::loadShadersFile(string filename) {

		if (settings->getGraphicsQuality() == RenderSettings::GRAPHICS_VERY_LOW)
			return;

		string directory = DirectoryFunctions::extractDirectory(filename);
		LOG(OPENGL, "Loading Shader File " << filename);

		ifstream in_file;
		in_file.open(filename.c_str());
		while (!in_file.eof()) {
			string shader_type;
			in_file >> shader_type;
			if (shader_type[0] == '#') {
				//Ignore lines starting with #
				in_file.ignore(9999,'\n');
			}
			if (shader_type == "all"
				|| (shader_type == "high" && settings->getGraphicsQuality() >= RenderSettings::GRAPHICS_HIGH)
				|| (shader_type == "med" && settings->getGraphicsQuality() == RenderSettings::GRAPHICS_MED)
				|| (shader_type == "low" && settings->getGraphicsQuality() <= RenderSettings::GRAPHICS_LOW)) {
				string shader_name;
				in_file >> shader_name;
				if (shader_name.length() > 0) {

					string frag_file;
					string vert_file;
					in_file >> frag_file;
					in_file >> vert_file;
					if (frag_file == "mapping") {
						struct ShaderMapping mapping;
						mapping.name = shader_name;
						mapping.mapped_name = vert_file;
						shaderMappings.push_back(mapping);
					}
					else {
						loadShader(shader_name,
							Misc::DirectoryFunctions::toRelativeFilename(directory, frag_file),
							Misc::DirectoryFunctions::toRelativeFilename(directory, vert_file));
					}

				}
			}
		}
		in_file.close();
		
	}
	void RenderManager::loadShader(string name, string fragment_file, string vertex_file) {

		if (settings->getGraphicsQuality() == RenderSettings::GRAPHICS_VERY_LOW)
			return;

		if (name.length() > 0) {
			Shader::ShaderProgram* new_shader = new Shader::ShaderProgram((GLchar*) fragment_file.c_str(),(GLchar*) vertex_file.c_str());
			new_shader->turnShaderOff();
			shader.push_back(new_shader);
			shaderName.push_back(name);
			
			vector< ShaderAttributeLocation > attributes;

			//Temporary solution to Attribute problem
			//Preload potential vertex shader attribute locations
			int loc = new_shader->getAttrLoc(SHADER_TANGENT_ATTRIBUTE_NAME);
			if (loc > -1)
				attributes.push_back(ShaderAttributeLocation(SHADER_TANGENT_ATTRIBUTE_NAME, loc));

			loc = new_shader->getAttrLoc(SHADER_BITANGENT_ATTRIBUTE_NAME);
			if (loc > -1)
				attributes.push_back(ShaderAttributeLocation(SHADER_BITANGENT_ATTRIBUTE_NAME, loc));

			LOG(OPENGL, "Shader loaded: " << name);
			LOG(OPENGL, "	Fragment Shader: " << fragment_file);
			LOG(OPENGL, "	Vertex Shader: " << vertex_file);
			shaderAttributeLocation.push_back(attributes);
		}
		else {
			LOG(OPENGL, "Empty shader name provided");
		}
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
		//paramSetter.setHasTangentSpace(false);
	}

	Shader::ShaderProgram* RenderManager::getShaderByIndex(int shader_index) {
		if (shader_index < 0 || static_cast<unsigned int>(shader_index) > shader.size())
			return NULL;
		
		return shader[shader_index];
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

		for (unsigned int i = 0; i < shaderParams.size(); i++) {
			for (unsigned int j = 0; j < shaderParams[i].size(); j++) {
				shaderParams[i][j]->setShaderParameters(paramSetter);
			}
		}

		//Get tangent space locations
		tangentLocation = getShaderAttributeLocation(enabledShaderIndex, SHADER_TANGENT_ATTRIBUTE_NAME);
		bitangentLocation = getShaderAttributeLocation(enabledShaderIndex, SHADER_BITANGENT_ATTRIBUTE_NAME);
		//paramSetter.setHasTangentSpace( tangentLocation >= 0 || bitangentLocation >= 0 );


		//Set standard parameters
		setUniformInt(SHADER_NUMLIGHTS_UNIFORM_NAME, lightManager->getNumActiveLights());
		if (cubeMap && camera) {
			glActiveTexture(cubeMapTexture);
			glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap->getCubeMap());
			glDisable(GL_TEXTURE_CUBE_MAP);
			glActiveTexture(colorMapTexture);
			setUniformInt(SHADER_CUBEMAP_UNIFORM_NAME, cubeMapTextureNum);
			setUniformMatrix4(SHADER_CAMERA_MATRIX_UNIFORM_NAME, GL_FALSE, camera->getCameraMatrix());
		}

	}
	*/

	void RenderManager::setShaderParameters() {

		if (!shaderManager->getIsShaderActive())
			return;

		//Set standard shader parameters
		const Shader::ShaderParamSetter& setter = shaderManager->getShaderParamSetter();
		//Number of lights
		setter.setStandardParamInt(Shader::ShaderParameter::UNIFORM,
			static_cast<int>(Shader::ShaderManager::UINT_NUM_LIGHTS),
			lightManager->getNumActiveLights());
		//Color map
		setter.setStandardParamInt(Shader::ShaderParameter::UNIFORM,
			static_cast<int>(Shader::ShaderManager::UINT_COLOR_MAP),
			COLOR_MAP_TEXTURE_INDEX);
		//Normal map
		setter.setStandardParamInt(Shader::ShaderParameter::UNIFORM,
			static_cast<int>(Shader::ShaderManager::UINT_NORMAL_MAP),
			NORMAL_MAP_TEXTURE_INDEX);
		//Glow map
		setter.setStandardParamInt(Shader::ShaderParameter::UNIFORM,
			static_cast<int>(Shader::ShaderManager::UINT_GLOW_MAP),
			GLOW_MAP_TEXTURE_INDEX);
		//Has textures array
		setter.setStandardParamIntArray(Shader::ShaderParameter::UNIFORM,
			static_cast<int>(Shader::ShaderManager::UINTV_HAS_TEXTURE),
			hasTexture, NUM_STANDARD_TEXTURES);

		if (cubeMap) {
			glActiveTexture(CUBE_MAP_TEXTURE_UNIT);
			glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap->getCubeMap());
			glActiveTexture(COLOR_MAP_TEXTURE_UNIT);
			//Cube map
			setter.setStandardParamInt(Shader::ShaderParameter::UNIFORM,
				static_cast<int>(Shader::ShaderManager::UINT_CUBE_MAP),
				CUBE_MAP_TEXTURE_INDEX);
		}

		if (camera) {
			//Camera matrix
			setter.setStandardParamMatrix4(Shader::ShaderParameter::UNIFORM,
				static_cast<int>(Shader::ShaderManager::UM4_CAMERA_MATRIX),
				GL_FALSE,
				camera->getCameraMatrix());
		}

		//Set custom shader parameters
		for (unsigned int i = 0; i < shaderParams.size(); i++) {
			for (unsigned int j = 0; j < shaderParams[i].size(); j++) {
				shaderParams[i][j]->set(setter);
			}
		}
		
		
	}

	const BoundingObject* RenderManager::getBoundingObject() const {
		if (camera)
			return camera->getFrustrum();
		else
			return NULL;
	}

	void RenderManager::setCubeMap(TextureCube* cube_map) {
		cubeMap = cube_map;
	}

	bool RenderManager::hasTransformation() const {
		return (numTransformations > 0);
	}
	

}  // namespace Render
}  // namespace Project
