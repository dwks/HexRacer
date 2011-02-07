#include "GUIInputManager.h"

#include "widget/MouseMoveEvent.h"
#include "widget/MouseButtonEvent.h"
#include "widget/KeyEvent.h"

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
        bool down = (event->type == SDL_KEYDOWN);
        
        Widget::KeyEvent widgetEvent(key, down);
        gui->handleEvent(&widgetEvent);
    }
    default:
        break;
    }
}

}  // namespace SDL
}  // namespace Project
