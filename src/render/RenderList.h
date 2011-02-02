#ifndef PROJECT_RENDER__RENDER_LIST_H
#define PROJECT_RENDER__RENDER_LIST_H

#include "RenderableObject.h"
#include <vector>

namespace Project {
namespace Render {

class RenderList
	: public RenderableObject {
private:
	vector<RenderableObject*> renderableList;
public:
	void addRenderable(RenderableObject* renderable);
	bool removeRenderable(RenderableObject* renderable);
	void clear() { renderableList.clear(); }

	void subRender(RenderManager* manager);
};

}  // namespace Render
}  // namespace Project

#endif
