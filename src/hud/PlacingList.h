#ifndef PROJECT_HUD__PLACING_LIST_H
#define PROJECT_HUD__PLACING_LIST_H

#include "map/RaceManager.h"
#include "render/TextureObjectChain.h"

namespace Project {
namespace HUD {

class PlacingList {
private:
	double aspect;
	double entryHeight;
	double totalHeight;
	double totalWidth;
	Render::TextureObjectChain entry;
public:
	PlacingList();

	void setAspectRatio(double _aspect) { aspect = _aspect; }
	void setEntryHeight(double entry_height) { entryHeight = entry_height; }
	void setTotalHeight(double total_height) { totalHeight = total_height; }
	void setTotalWidth(double total_width) { totalWidth = total_width; }

	void render(Map::RaceManager* race_manager);
};

}  // namespace HUD
}  // namespace Project

#endif
