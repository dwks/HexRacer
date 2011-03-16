#include "GUIInputManager.h"

#include "widget/MouseMoveEvent.h"
#include "widget/MouseButtonEvent.h"
#include "widget/KeyEvent.h"
#include "widget/MenuMoveEvent.h"
#include "input/GlobalInputManager.h"

namespace Project {
namespace SDL {

void GUIInputManager::handleEvent(SDL_Event *event) {
    int width = SDL_GetVideoSurface()->w;
    int height = SDL_GetVideoSurface()->h;
    
    switch(event->type) {
    case SDL_MOUSEMOTION: {
        double x = event->motion.x / double(width);
        double y = event->motion.y / double(height);
        Widget::WidgetPoint point(x, y);
        
        Widget::MouseMoveEvent widgetEvent(point);
        gui->handleEvent(&widgetEvent);
        
        break;
    }
    case SDL_MOUSEBUTTONDOWN:
    case SDL_MOUSEBUTTONUP: {
        double x = event->button.x / double(width);
        double y = event->button.y / double(height);
        Widget::WidgetPoint point(x, y);
        
        Widget::MouseButtonEvent::button_t button
            = static_cast<Widget::MouseButtonEvent::button_t>(
                event->button.button);
        
        bool down = (event->type == SDL_MOUSEBUTTONDOWN);
        
        Widget::MouseButtonEvent widgetEvent(point, down, button);
        gui->handleEvent(&widgetEvent);
        
        break;
    }
    case SDL_KEYDOWN:
    case SDL_KEYUP: {
        long key = event->key.keysym.sym;
        long unicode = event->key.keysym.unicode;
        bool down = (event->type == SDL_KEYDOWN);
        
        Widget::KeyEvent widgetEvent(key, unicode, down);
        gui->handleEvent(&widgetEvent);
    }
    default:
        break;
    }
}

void GUIInputManager::generateMenuEvents() {

	Input::InputMapper* mapper = Input::GlobalInputManager::getInstance()->getInputMapper();

	mapper->update(Input::INPUT_D_MENU_LEFT);
	mapper->update(Input::INPUT_D_MENU_RIGHT);
	mapper->update(Input::INPUT_D_MENU_UP);
	mapper->update(Input::INPUT_D_MENU_DOWN);
	mapper->update(Input::INPUT_D_MENU_CONFIRM);
	mapper->update(Input::INPUT_D_MENU_BACK);

	Widget::MenuMoveEvent* move_event = new Widget::MenuMoveEvent();

	if (mapper->getDigitalTriggered(Input::INPUT_D_MENU_LEFT))
		move_event->xDir = Widget::MenuMoveEvent::LEFT;
	else if (mapper->getDigitalTriggered(Input::INPUT_D_MENU_RIGHT))
		move_event->xDir = Widget::MenuMoveEvent::RIGHT;

	if (mapper->getDigitalTriggered(Input::INPUT_D_MENU_UP))
		move_event->yDir = Widget::MenuMoveEvent::UP;
	else if (mapper->getDigitalTriggered(Input::INPUT_D_MENU_DOWN))
		move_event->yDir = Widget::MenuMoveEvent::DOWN;

	if (move_event->xDir != Widget::MenuMoveEvent::XNONE || move_event->yDir != Widget::MenuMoveEvent::YNONE) {
		gui->handleEvent(move_event);
	}

}

}  // namespace SDL
}  // namespace Project
