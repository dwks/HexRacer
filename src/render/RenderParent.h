#ifndef PROJECT_RENDER__RENDER_PARENT_H
#define PROJECT_RENDER__RENDER_PARENT_H

#include "RenderableObject.h"

namespace Project {
namespace Render {

class RenderParent
	: public RenderableObject {
private:
	RenderableObject* child;
public:
	RenderParent(RenderableObject* _child = NULL);
	void subRender(RenderManager* manager);
	void setChild(RenderableObject* _child) { child = _child; }
};

}  // namespace Render
}  // namespace Project

#endif
