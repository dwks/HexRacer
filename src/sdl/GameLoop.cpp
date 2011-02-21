#include "SDL.h"  // for SDL_GetTicks()

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

void GameLoop::setGuiPointers(boost::shared_ptr<GUI::GUISystem> gui,
    boost::shared_ptr<GUIInputManager> guiInputManager) {
    
    this->gui = gui;
    this->guiInputManager = guiInputManager;
    
    gameRenderer->setGUI(gui);
}

void GameLoop::handleEvent(SDL_Event *event) {
    viewport->checkForDebugCameraEvents(event);
    
    inputManager->handleEvent(event);
    
    guiInputManager->handleEvent(event);
}

void GameLoop::miscellaneous() {
    gameWorld->checkNetwork();
    
    paintSubsystem->doStep(SDL_GetTicks());
    
    // must do paused (static) checks first, in case the game unpauses
    inputManager->doPausedChecks();
    inputManager->doStep(SDL_GetTicks());
    
    gameWorld->doPhysics();
	gameWorld->updatePlayerPathing();
    
    viewport->doCamera(SDL_GetTicks());
}

void GameLoop::render() {

    gameRenderer->render(
        viewport->getCamera(),
        gameWorld->getWorldManager());

	gameRenderer->renderDebug(
		viewport->getCamera(),
        gameWorld->getWorldManager(),
        gameWorld->getWorldManager()->getPlayer(
            gameWorld->getClientData()->getPlayerID()));

	gameWorld->render();
    
    gameRenderer->renderHUD(
        gameWorld->getWorldManager(),
        gameWorld->getWorldManager()->getPlayer(
            gameWorld->getClientData()->getPlayerID()));

    gui->render();
}

void GameLoop::setProjection(const Point2D &size) {
    viewport->setProjection(size);
}

}  // namespace SDL
}  // namespace Project
