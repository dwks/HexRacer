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
		shadowMap = 0;

		activeShaderChanged = true;

		for (int i = 0; i < NUM_STANDARD_TEXTURES; i++)
			hasTexture[i] = 0;

		lightManager = new LightManager(settings->getMaxLights());

		LOG(OPENGL, "Renderer Initialized.");

		LOG(OPENGL, "System Video Capabilities:");
		GLint value;
		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &value);
		LOG(OPENGL, "	Max texture image units: " << value);
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &value);
		LOG(OPENGL, "	Max texture size: " << value);
		glGetIntegerv(GL_MAX_VARYING_FLOATS, &value);
		LOG(OPENGL, "	Max varying floats: " << value);

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

		if (properties->hasMaterialTint()) {
			materialTintStack.push(properties->getMaterialTint());
		}

		if (properties->hasMaterial()) {

			if (!materialsOverridden()) {
				//Apply the new material if not overridden
				materialStack.push(properties->getMaterial());
				if (materialTintStack.empty())
					properties->getMaterial()->glApply();
				else {
					(*(materialTintStack.top())*(*(properties->getMaterial()))).glApply();
				}
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
					activeShaderChanged = true;
				}
			}
		}

		if (properties->getShaderOverride())
			numShaderOverrides++;
	}

	void RenderManager::setRenderTexture(RenderProperties* properties) {

		hasTexture[COLOR_MAP_TEXTURE_INDEX] = 0;
		hasTexture[NORMAL_MAP_TEXTURE_INDEX] = 0;
		hasTexture[GLOW_MAP_TEXTURE_INDEX] = 0;
		hasTexture[SHADOW_MAP_TEXTURE_INDEX] = (int)(shadowMap > 0);

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

		if (properties->getColorOverride())
			numColorOverrides--;

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
	}
	void RenderManager::revertRenderMaterial(RenderProperties* properties) {

		if (properties->getMaterialOverride())
			numMaterialOverrides--;

		if (properties->hasMaterialTint()) {
			materialTintStack.pop();
		}

		if (properties->hasMaterial()) {

			if (!materialsOverridden()) {
				materialStack.pop();
				//Reapply the previous material
				if (!materialStack.empty()) {
					if (materialTintStack.empty())
						materialStack.top()->glApply();
					else {
						(*(materialTintStack.top())*(*(materialStack.top()))).glApply();
					}
				}
			}
		}
	}
	void RenderManager::revertRenderShader(RenderProperties* properties) {

		if (properties->getShaderOverride())
			numShaderOverrides--;

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

	}

	void RenderManager::revertRenderTexture(RenderProperties* properties) {
		glActiveTexture(COLOR_MAP_TEXTURE_UNIT);
		glDisable(GL_TEXTURE_2D);
		if (properties->getTexturePackOverride())
			numTextureOverrides--;
	}

	void RenderManager::setShaderParameters() {

		if (!shaderManager->getIsShaderActive())
			return;

		//Set standard shader parameters
		const Shader::ShaderParamSetter& setter = shaderManager->getShaderParamSetter();
		//Number of lights
		setter.setStandardParamInt(Shader::ShaderParameter::UNIFORM,
			static_cast<int>(Shader::ShaderManager::UINT_NUM_LIGHTS),
			lightManager->getNumActiveLights());
		//Has textures array
		setter.setStandardParamIntArray(Shader::ShaderParameter::UNIFORM,
			static_cast<int>(Shader::ShaderManager::UINTV_HAS_TEXTURE),
			hasTexture, NUM_STANDARD_TEXTURES);

		if (activeShaderChanged) {

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

			if (shadowMap > 0) {
				glActiveTexture(SHADOW_MAP_TEXTURE_UNIT);
				glBindTexture(GL_TEXTURE_2D, shadowMap);
				//Shadow map
				setter.setStandardParamInt(Shader::ShaderParameter::UNIFORM,
					static_cast<int>(Shader::ShaderManager::UINT_SHADOW_MAP),
					SHADOW_MAP_TEXTURE_INDEX);
			}
			if (cubeMap) {
				glActiveTexture(CUBE_MAP_TEXTURE_UNIT);
				glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap->getCubeMap());
				//Cube map
				setter.setStandardParamInt(Shader::ShaderParameter::UNIFORM,
					static_cast<int>(Shader::ShaderManager::UINT_CUBE_MAP),
					CUBE_MAP_TEXTURE_INDEX);
			}

			glActiveTexture(COLOR_MAP_TEXTURE_UNIT);

			if (camera) {
				//Camera matrix
				setter.setStandardParamMatrix4(Shader::ShaderParameter::UNIFORM,
					static_cast<int>(Shader::ShaderManager::UM4_CAMERA_MATRIX),
					GL_FALSE,
					camera->getCameraMatrix());
			}

			activeShaderChanged = false;
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
