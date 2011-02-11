#ifndef PROJECT_RENDER__BACKGROUND_RENDERABLE_H
#define PROJECT_RENDER__BACKGROUND_RENDERABLE_H

#include "BaseRenderable.h"
#include "opengl/Camera.h"

namespace Project {
namespace Render {

/** A renderable that draws a flat plane in the background of a camera
*/
class BackgroundRenderable
	: public BaseRenderable {
private:
	OpenGL::Camera* camera;
public:
	BackgroundRenderable(OpenGL::Camera* _camera) { camera = _camera;}
	void setCamera(OpenGL::Camera* _camera) { camera = _camera; }
	void renderGeometry(const Shader::ShaderParamSetter& setter, const Math::BoundingObject* bounding_object = NULL);
};

}  // namespace Render
}  // namespace Project

#endif
