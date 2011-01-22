#ifndef PROJECT_PHYSICS__SUSPENSION_H
#define PROJECT_PHYSICS__SUSPENSION_H

#include "render/RenderManager.h"
#include "object/PlayerList.h"

namespace Project {
namespace Physics {

class Suspension {
public:
    /** If @a renderManager is NULL, no debug drawing will be done.
    */
    void applySuspension(Object::PlayerList *playerList,
        Render::RenderManager *renderManager);
};

}  // namespace Physics
}  // namespace Project

#endif
