#include "MotionState.h"
#include "Converter.h"

#include "log/Logger.h"

namespace Project {
namespace Physics {

MotionState::MotionState(const btTransform &graphicsWorldTransform,
    const btTransform &centreOfMassOffset) {
    
    this->graphicsWorldTransform = graphicsWorldTransform;
    this->centreOfMassOffset = centreOfMassOffset;
}

void MotionState::getWorldTransform(btTransform &centreOfMassWorldTrans) const {
    centreOfMassWorldTrans
        = centreOfMassOffset.inverse() * graphicsWorldTransform;
}

void MotionState::setWorldTransform(const btTransform &centreOfMassWorldTrans) {
    //LOG(PHYSICS, Converter::toMatrix(graphicsWorldTransform));
    graphicsWorldTransform
        = centreOfMassWorldTrans * centreOfMassOffset;
}

}  // namespace Physics
}  // namespace Project
