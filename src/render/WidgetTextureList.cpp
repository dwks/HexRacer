#include "WidgetTextureList.h"
#include "Texture.h"

#include "settings/SettingsManager.h"
#include "log/Logger.h"

namespace Project {
namespace Render {

WidgetTextureList *WidgetTextureList::instance = NULL;

WidgetTextureList::WidgetTextureList() {
    instance = this;
    
    std::string theme = GET_SETTING("gui.theme", "");
    addTexture("corners/in/normal", theme + "in-normal.png");
    addTexture("corners/in/hover", theme + "in-hover.png");
    addTexture("corners/in/active", theme + "in-active.png");
    
    addTexture("corners/out/normal", theme + "out-normal.png");
    addTexture("corners/out/hover", theme + "out-hover.png");
    addTexture("corners/out/active", theme + "out-active.png");
}

void WidgetTextureList::addTexture(const std::string &name, GLuint texture) {
    textureList[name] = texture;
}

void WidgetTextureList::addTexture(const std::string &name,
    const std::string &filename) {
    
    addTexture(name, Texture::loadTexture2D(filename));
}

GLuint WidgetTextureList::getTexture(const std::string &name) {
    texture_list_t::iterator found = textureList.find(name);
    if(found == textureList.end()) {
        LOG2(WIDGET, WARNING, "Widget texture \"" << name << "\" not found");
        return -1;
    }
    else {
        return (*found).second;
    }
}

}  // namespace Render
}  // namespace Project
