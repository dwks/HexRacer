#ifndef PROJECT_SDL__MENU_BACKGROUND_H
#define PROJECT_SDL__MENU_BACKGROUND_H

namespace Project {
namespace SDL {

class MenuBackground {
private:
    double rotate;
public:
    MenuBackground();
    
    void render();
};

}  // namespace SDL
}  // namespace Project

#endif
