#ifndef PROJECT_RENDER__SHADER_PARAMETER_H
#define PROJECT_RENDER__SHADER_PARAMETER_H

#include "ShaderParamSetter.h"

namespace Project {
namespace Render {

class ShaderParameter {
public:
	virtual void setShaderParameters(ShaderParamSetter& setter) = 0;
protected:
	const char * name;
};

}  // namespace Render
}  // namespace Project

#endif
