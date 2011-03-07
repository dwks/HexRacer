#include "FontManager.h"

#include "settings/SettingsManager.h"
#include "log/Logger.h"

namespace Project {
namespace Render {

FontManager *FontManager::instance = 0;

FontManager::FontManager() {
    instance = this;
    
    LOG(SDL, "Loading fonts");
    loadFonts();
}

FontManager::~FontManager() {
    LOG(SDL, "Deleting fonts");
    for(FontListType::iterator i = fontList.begin(); i != fontList.end();
        ++ i) {
        
        TTF_Font *font = (*i).second;
        TTF_CloseFont(font);
    }
}

void FontManager::loadFonts() {
    TTF_Font *font = TTF_OpenFont(
        GET_SETTING("font", "data/gui/DejaVuSans.ttf").c_str(),
        GET_SETTING("font.size", 16));
    
    if(!font) {
        LOG2(GUI, ERROR, "Can't load normal font! "
            << GET_SETTING("font", "data/DejaVuSans.ttf"));
    }
    
    fontList["normal"] = font;
    currentKind = "normal";
}

void FontManager::useFont(const std::string &kind) {
    currentKind = kind;
}

TTF_Font *FontManager::getCurrentFont() {
    return fontList[currentKind];
}

TTF_Font *FontManager::getFont(const std::string &kind) {
    return fontList[kind];
}

}  // namespace Render
}  // namespace Project
