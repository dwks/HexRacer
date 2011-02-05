#include "GameLoop.h"

namespace Project {
namespace SDL {

GameLoop::GameLoop(const std::string &host, unsigned short port) {
    gameWorld = boost::shared_ptr<GameWorld>(new GameWorld());
    gameWorld->construct(host, port);
}

void GameLoop::construct() {
    viewport = boost::shared_ptr<GameViewport>(new GameViewport());
    viewport->setPlayerManager(gameWorld->getPlayerManager());
    
    gameRenderer = boost::shared_ptr<GameRenderer>(new GameRenderer());
    gameRenderer->construct(viewport->getCamera());
    
    gameWorld->construct2(gameRenderer->getMap());
    
    paintSubsystem = boost::shared_ptr<Paint::PaintSubsystem>(
        new Paint::PaintSubsystem(
            gameWorld->getWorldManager(),
            gameRenderer->getPaintManager(), 20));
    
    inputManager = boost::shared_ptr<InputManager>(
        new InputManager(10, gameWorld->getClientData()));
    inputManager->init();
}

void GameLoop::handleEvent(SDL_Event *event) {
    inputManager->handleEvent(event);
}

void GameLoop::render() {
    gameRenderer->render(
        viewport->getCamera(),
        gameWorld->getWorldManager()->getWorld());
}

void GameLoop::setProjection(const Point2D &size) {
    viewport->setProjection(size);
}

}  // namespace SDL
}  // namespace Project
