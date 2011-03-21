#ifndef PROJECT_HUD__PLAYER_PLACING_TEXT_H
#define PROJECT_HUD__PLAYER_PLACING_TEXT_H

#include <string>

namespace Project {
namespace HUD {

class PlayerPlacingText {
private:
	int placing;
	std::string placingString;
public:
	PlayerPlacingText();
	void setPlacing(int _placing);
	void render();
};

}  // namespace HUD
}  // namespace Project

#endif
