#include "GameViewport.h"

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
}

GameViewport::~GameViewport() {
    delete cameraObject;
}

void GameViewport::setPlayerManager(PlayerManager *playerManager) {
    cameraObject->setPlayerManager(playerManager);
}

void GameViewport::setProjection(const Point2D &size) {
    if(cameraObject) {
        cameraObject->camera->setAspect(
            static_cast<double>(size.getX()) / size.getY());
        cameraObject->camera->glProjection();
    }
}

void GameViewport::updateCamera() {
    cameraObject->camera->setLookDirection(trackball->getSpherePoint());
}

}  // namespace SDL
}  // namespace Project
