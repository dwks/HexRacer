#ifndef PROJECT_SDL__MENU_BACKGROUND_H
#define PROJECT_SDL__MENU_BACKGROUND_H

#include "math/HexGrid.h"
#include "SDL_events.h"
#include <map>

namespace Project {
namespace SDL {

#define BG_HEX_RADIUS 0.075

class MenuBackground {
private:
	int bandTimer;
	int bandSettleTimer;
	float bandBrightness;
    double rotate;
	Math::HexGrid hexGrid;
	std::map< Math::HexGrid::HexIndex, float > hexBrightness;
public:
    MenuBackground();
	~MenuBackground();
	void handleEvent(SDL_Event *event);
    
    void render();
};

}  // namespace SDL
}  // namespace Project

#endif
