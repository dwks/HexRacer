#include "ShaderManager.h"
#include "misc/DirectoryFunctions.h"
#include "log/Logger.h"
#include <fstream>
using namespace std;

namespace Project {
namespace Shader {

	ShaderManager::ShaderManager(Render::RenderSettings& _settings)
		: settings(_settings) {
		activeShaderIndex =-1;
	}

	ShaderManager::~ShaderManager() {
		for (unsigned int i = 0; i < shaderPrograms.size(); i++)
			delete shaderPrograms[i];
		for (unsigned int i = 0; i < standardUniformLocations.size(); i++)
			delete [] standardUniformLocations[i];
		for (unsigned int i = 0; i < standardAttributeLocations.size(); i++)
			delete [] standardAttributeLocations[i];
	}

	void ShaderManager::loadShadersFile(string filename) {

		string shader_quality;

		switch (settings.getGraphicsQuality()) {
			case Render::RenderSettings::GRAPHICS_LOW: shader_quality = "low"; break;
			case Render::RenderSettings::GRAPHICS_MED: shader_quality = "med"; break;
			case Render::RenderSettings::GRAPHICS_HIGH: shader_quality = "high"; break;
			default: return;
		}

		string directory = Misc::DirectoryFunctions::extractDirectory(filename);
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
			if (shader_type == "all" || shader_type == shader_quality) {
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
	void ShaderManager::loadShader(string name, string fragment_file, string vertex_file) {

		if (name.length() > 0) {

			ShaderProgram* new_shader = new ShaderProgram((const GLchar*)fragment_file.c_str(), (const GLchar*)vertex_file.c_str());
			new_shader->turnShaderOff();
			shaderPrograms.push_back(new_shader);
			shaderNames.push_back(name);

			//Get the locations of standard shader parameters
			int* uniform_locs = new int [NUM_STANDARD_UNIFORMS];
			int* attribute_locs = new int [NUM_STANDARD_ATTRIBUTES];

			for (int i = 0; i < NUM_STANDARD_UNIFORMS; i++)
				uniform_locs[i] = new_shader->getUniLoc(standardUniformName(static_cast<StandardUniform>(i)));

			for (int i = 0; i < NUM_STANDARD_ATTRIBUTES; i++)
				attribute_locs[i] = new_shader->getAttrLoc(standardAttributeName(static_cast<StandardAttribute>(i)));

			standardUniformLocations.push_back(uniform_locs);
			standardAttributeLocations.push_back(attribute_locs);

			LOG(OPENGL, "Shader loaded: " << name);
			LOG(OPENGL, "	Fragment Shader: " << fragment_file);
			LOG(OPENGL, "	Vertex Shader: " << vertex_file);
		}
		else {
			LOG(OPENGL, "Empty shader name provided");
		}

	}

	void ShaderManager::enableShader(int shaderIndex) {
		if (shaderIndex < 0 || shaderIndex >= static_cast<int>(shaderPrograms.size()))
			disableShader();
		else {
			shaderPrograms[shaderIndex]->turnShaderOn();
			setter.setShaderProgram(
				shaderPrograms[shaderIndex],
				standardUniformLocations[shaderIndex],
				standardAttributeLocations[shaderIndex]
			);
			activeShaderIndex = shaderIndex;
		}
	}

	void ShaderManager::disableShader() {
		glUseProgram(NULL);
		setter.setShaderProgram(NULL, NULL, NULL);
		activeShaderIndex = -1;
	}

	int ShaderManager::shaderIndexFromName(string name) {

		for (unsigned int i = 0; i < shaderMappings.size(); i++) {
			if (shaderMappings[i].name == name) {
				name = shaderMappings[i].mapped_name;
			}
		}

		for (unsigned int i = 0; i < shaderNames.size(); i++) {
			if (shaderNames[i] == name)
				return i;
		}

		return -1;
		
	}
	const char* ShaderManager::standardUniformName(StandardUniform uniform) {
		switch (uniform) {
			case UINT_NUM_LIGHTS: return "numLights";
			case UINTV_HAS_TEXTURE: return "hasTexture";
			case UINT_COLOR_MAP: return "colorMap";
			case UINT_NORMAL_MAP: return "normalMap";
			case UINT_GLOW_MAP: return "glowMap";
			case UINT_SHADOW_MAP: return "shadowMap";
			case UINT_CUBE_MAP: return "cubeMap";
			case UM4_CAMERA_MATRIX: return "cameraMatrix";
			default: return "";
		}
	}

	const char* ShaderManager::standardAttributeName(StandardAttribute attribute) {
		switch (attribute) {
			case AV3_TANGENT: return "tangent";
			case AV3_BITANGENT: return "bitangent";
			default: return "";
		}
	}

}  // namespace Shader
}  // namespace Project
