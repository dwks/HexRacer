#include "RunningProxy.h"

#include "widget/WidgetBase.h"
#include "widget/KeyEvent.h"
#include "widget/WidgetActivateEvent.h"
#include "event/PauseGame.h"

#include "event/EventSystem.h"

#include "log/Logger.h"

#include "SDL_keysym.h"

namespace Project {
namespace GUI {

void RunningProxy::visit(Widget::KeyEvent *event) {
    if(event->getKey() == SDLK_ESCAPE && event->getDown()) {
        EMIT_EVENT(new Event::PauseGame(true));
    }
}

}  // namespace GUI
}  // namespace Project
