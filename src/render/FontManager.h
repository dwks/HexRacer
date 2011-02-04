#ifndef PROJECT_RENDER__FONT_MANAGER_H
#define PROJECT_RENDER__FONT_MANAGER_H

#include "SDL_ttf.h"

#include <string>
#include <map>

namespace Project {
namespace Render {

class FontManager {
private:
    static FontManager *instance;
public:
    static FontManager *getInstance() { return instance; }
private:
    typedef std::map<std::string, TTF_Font *> FontListType;
    FontListType fontList;
    
    std::string currentKind;
public:
    FontManager();
    ~FontManager();
    
    void useFont(const std::string &kind);
    
    TTF_Font *getCurrentFont();
    TTF_Font *getFont(const std::string &kind);
private:
    void loadFonts();
};

}  // namespace Render
}  // namespace Project

#endif
