#ifndef PROJECT_SHADER__SHADER_MANAGER_H
#define PROJECT_SHADER__SHADER_MANAGER_H

#include "ShaderProgram.h"
#include "ShaderParamSetter.h"
#include "render/RenderSettings.h"
#include <vector>

namespace Project {
namespace Shader {

class ShaderParamSetter;

class ShaderManager {
public:

	static const int NUM_STANDARD_UNIFORMS = 7;
	enum StandardUniform {
		UINT_NUM_LIGHTS,
		UINTV_HAS_TEXTURE,
		UINT_COLOR_MAP,
		UINT_NORMAL_MAP,
		UINT_GLOW_MAP,
		UINT_CUBE_MAP,
		UM4_CAMERA_MATRIX
	};
	static const int NUM_STANDARD_ATTRIBUTES = 2;
	enum StandardAttribute {
		AV3_TANGENT,
		AV3_BITANGENT
	};
	static const char* standardUniformName(StandardUniform uniform);
	static const char* standardAttributeName(StandardAttribute attribute);

private:

	struct ShaderMapping {
		string name;
		string mapped_name;
	};

	std::vector<ShaderProgram*> shaderPrograms;
	std::vector<std::string> shaderNames;
	std::vector<int*> standardUniformLocations;
	std::vector<int*> standardAttributeLocations;
	std::vector<ShaderMapping> shaderMappings;

	ShaderParamSetter setter;
	Render::RenderSettings& settings;
	int activeShaderIndex;

public:

	ShaderManager(Render::RenderSettings& _settings);
	~ShaderManager();

	void loadShadersFile(std::string filename);
	void loadShader(std::string name, string fragment_file, string vertex_file);

	void enableShader(int shaderIndex);
	void disableShader();

	int getActiveShaderIndex() const { return activeShaderIndex; }
	bool getIsShaderActive() const { return activeShaderIndex >= 0; }

	int shaderIndexFromName(std::string name);

	const ShaderParamSetter& getShaderParamSetter() const { return setter; }

};

}  // namespace Shader
}  // namespace Project

#endif
