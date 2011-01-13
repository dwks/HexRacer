#ifndef PROJECT_RENDER__RENDERABLE_OBJECT_H
#define PROJECT_RENDER__RENDERABLE_OBJECT_H

namespace Project {
namespace Render {

class RenderableObject {
public:

	bool hasTransformation() const;
	void clearTransformation();
	void setTransformation(Project::Math::Matrix transform);
	Project::Math::Matrix getTransformation() const;

};

}  // namespace Render
}  // namespace Project

#endif
