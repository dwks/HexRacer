#include "Suspension.h"

#include "object/Player.h"
#include "object/PlayerList.h"
#include "settings/SettingsManager.h"

#include "PhysicsWorld.h"

namespace Project {
namespace Physics {

void Suspension::applySuspension(Object::PlayerList *playerList,
    Render::RenderManager *renderManager) {
    
    if(!GET_SETTING("physics.driving.dosuspension", 0)) {
        return;
    }
    
    Object::PlayerList::IteratorType it = playerList->getIterator();
    while(it.hasNext()) {
        Object::Player *player = it.next();
        
        static const Math::Point suspensionPoint[] = {
            Math::Point(5.0, -2.0, 10.0) / 20.0,
            Math::Point(-5.0, -2.0, 10.0) / 20.0,
            Math::Point(-5.0, -2.0, -10.0) / 20.0,
            Math::Point(5.0, -2.0, -10.0) / 20.0
        };
        
        for(int wheel = 0; wheel < 4; wheel ++) {
            Math::Matrix matrix = player->getTransformation();
            
            //LOG(OPENGL, "Centre of mass: " << matrix * Math::Point(0.0, 0.0, 0.0, 1.0));
            
            /*matrix = Math::Matrix::getTranslationMatrix(
                suspensionPoint[wheel]) * matrix;*/
            
            // uses 4D points
            Math::Point axis = matrix * Math::Point(0.0, -1.0, 0.0, 0.0);
            //Math::Point point = matrix * Math::Point(0.0, 0.0, 0.0, 1.0);
            Math::Point point = matrix * suspensionPoint[wheel];
            
            //LOG(OPENGL, "Wheel " << wheel << ": " << point);
            
            Physics::PhysicsWorld::getInstance()->getDebug()
                .drawLine(Physics::Converter::toVector(point),
                    Physics::Converter::toVector(point + axis), btVector3(1.0, 0.0, 0.0));
            
            const double REST_LENGTH
                = GET_SETTING("physics.driving.restlength", 1.0);
            const double STRETCH_LENGTH
                = GET_SETTING("physics.driving.stretchlength", 1.0);
            
            double length = Physics::PhysicsWorld::getInstance()
                ->raycastLength(point, point + axis);
            
            length = length - REST_LENGTH;
            if(length < -STRETCH_LENGTH) length = -STRETCH_LENGTH;
            if(length > +STRETCH_LENGTH) length = 0.0; //length = +STRETCH_LENGTH;
            
            static const double K = 20.0; //(9.81 * 1.0) / (REST_LENGTH * 4);
            double factor = K * length;
            
            //if(factor > -4.5) factor = (factor + 4.5) * 2.0 - 4.5;
            
            if(factor < -5.0) factor = -5.0;
            
            //player->applyForce(Math::Point(0.0, 1.0, 0.0) * K, suspensionPoint[wheel]);
            //player->applyForce(axis * factor, suspensionPoint[wheel]);
            
            if(renderManager) {
                Physics::PhysicsWorld::getInstance()->getDebug()
                    .drawLine(Physics::Converter::toVector(point),
                        Physics::Converter::toVector(point + axis), btVector3(1.0, 0.0, 0.0));
            }
            
            //factor = (factor + 4.5) * 0.5 - 4.5;
            
            //LOG(PHYSICS, "force: " << factor << " * " << axis);
            player->applyForce(axis * factor, suspensionPoint[wheel]);
        }
    }
}

}  // namespace Physics
}  // namespace Project
