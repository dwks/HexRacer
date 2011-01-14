#ifndef PROJECT_RENDER__SHADER_PARAM_SETTER_H
#define PROJECT_RENDER__SHADER_PARAM_SETTER_H

#include "math/Point.h"
#include "opengl/Color.h"

namespace Project {
namespace Render {

class RenderManager;

class ShaderParamSetter {
public:

	ShaderParamSetter();
	ShaderParamSetter(RenderManager* manager);

	void setUniformVector3(const char *name, Project::Math::Point point);
	void setUniformVector4(const char *name, Project::OpenGL::Color color);
	void setAttributeVector3(const char *name, Project::Math::Point point);
	void setAttributeVector4(const char *name, Project::OpenGL::Color color);

private:

	RenderManager* manager;

};

}  // namespace Render
}  // namespace Project

#endif
