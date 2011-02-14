#include "GameViewport.h"

#include "Projector.h"

#include "event/ObserverList.h"

namespace Project {
namespace SDL {

void GameViewport::CameraObserver::observe(Event::CameraMovement *event) {
    double x = event->getMovement().getX();
    double y = event->getMovement().getY();
    
    Math::Point translation = camera->getLookDirection()*(y * 0.25f)
        + camera->getRightDirection() * (x * 0.25f);
    
    camera->translate(translation);
}

GameViewport::GameViewport() {
    trackball = new OpenGL::SimpleTrackball();
    
    cameraObject = new CameraObject();
    cameraObject->camera->setFieldOfViewDegrees(
        GET_SETTING("render.camera.fieldofview", 60.0));
    cameraObject->camera->setPosition(
        Point(0.0f, 2.0f, -4.0f));
    updateCamera();
    
    ADD_OBSERVER(new CameraObserver(trackball, cameraObject->camera));
}

GameViewport::~GameViewport() {
    delete cameraObject;
}

void GameViewport::setPlayerManager(PlayerManager *playerManager) {
    cameraObject->setPlayerManager(playerManager);
}

void GameViewport::setProjection(const Point2D &size) {
    if (cameraObject) {
        cameraObject->camera->setAspect(
            static_cast<double>(size.getX()) / size.getY());
        cameraObject->camera->glProjection();
    }
}

void GameViewport::updateCamera() {
    cameraObject->camera->setLookDirection(trackball->getSpherePoint());
}

void GameViewport::checkForDebugCameraEvents(SDL_Event *event) {
    Projector *projector = Projector::getInstance();
    
    switch(event->type) {
    case SDL_MOUSEBUTTONDOWN:
        if(event->button.button == 3) {  // right
            LOG(SDL, "Begin debug camera movement");
            
            trackball->setMouseStartAt(projector->screenToGL(
                Point2D(event->button.x, event->button.y)));
            
            trackball->setSpherePoint(
                cameraObject->camera->getLookDirection());
        }
        break;
    case SDL_MOUSEMOTION:
        if(event->motion.state & SDL_BUTTON(3)) {
            trackball->setMouseCurrentAt(projector->screenToGL(
                Point2D(event->button.x, event->button.y)));
            updateCamera();
        }
        break;
    }
}

void GameViewport::doCamera(unsigned long milliseconds) {
    //cameraObject->doAction(milliseconds);
}

}  // namespace SDL
}  // namespace Project
