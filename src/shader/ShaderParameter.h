#ifndef PROJECT_RENDER__SHADER_PARAMETER_H
#define PROJECT_RENDER__SHADER_PARAMETER_H
namespace Project {
namespace Shader {

class ShaderParamSetter;

class ShaderParameter {
public:
	enum ShaderParamType {UNIFORM, ATTRIBUTE};
protected:
	const char* name;
	ShaderParamType type;
public:
	ShaderParameter(const char* _name, ShaderParamType _type);
	virtual void set(const ShaderParamSetter& setter) = 0;
};

}  // namespace Render
}  // namespace Project

#endif
