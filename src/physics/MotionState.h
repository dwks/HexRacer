#ifndef PROJECT_PHYSICS__MOTION_STATE_H
#define PROJECT_PHYSICS__MOTION_STATE_H

#include "LinearMath/btMotionState.h"

namespace Project {
namespace Physics {

/** MotionState class, currently used for players.
*/
class MotionState : public btMotionState {
private:
    btTransform graphicsWorldTransform;
    btTransform centreOfMassOffset;
public:
    MotionState(const btTransform &graphicsWorldTransform,
        const btTransform &centreOfMassOffset = btTransform::getIdentity());
    
    virtual void getWorldTransform(btTransform &centreOfMassWorldTrans) const;
    virtual void setWorldTransform(const btTransform &centreOfMassWorldTrans);
};

}  // namespace Physics
}  // namespace Project

#endif
