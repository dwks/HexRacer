#ifndef PROJECT_RENDER__RENDER_LIST_H
#define PROJECT_RENDER__RENDER_LIST_H

#include "CompositeRenderable.h"
#include <vector>

namespace Project {
namespace Render {

class RenderList
	: public CompositeRenderable {
private:
	vector<RenderableObject*> renderableList;
public:
	void addRenderable(RenderableObject* renderable);
	bool removeRenderable(RenderableObject* renderable);
	vector<RenderableObject*> getChildren();
};

}  // namespace Render
}  // namespace Project

#endif
