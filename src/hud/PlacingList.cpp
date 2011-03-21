#include "PlacingList.h"
#include "opengl/OpenGL.h"
#include "opengl/GeometryDrawing.h"
#include "render/FontManager.h"
#include "render/StringTextureCache.h"
#include "render/ColorConstants.h"
#include "misc/StdVectorFunctions.h"

namespace Project {
namespace HUD {

	PlacingList::PlacingList()
		: entry(Render::TextureObjectChain::HORIZONTAL,
			Render::TextureObjectChain::MIN,
			Render::TextureObjectChain::MID) {
		aspect = 1.0;
	}

	void PlacingList::render(Map::RaceManager* race_manager) {

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0.0, totalWidth, 0.0, totalHeight, -1.0, 1.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		
		
		const std::vector<Object::Player*> players = race_manager->getPlayerRankings();

		double y = totalHeight;

		double hex_radius = entryHeight*0.5;
		double hex_half_height = hex_radius*sqrt(3.0)*0.5;
		//double hex_y_offset = entryHeight*0.5;
		double x_offset;
		double name_height = entryHeight*0.75;
		double y_increment = hex_half_height*2.0;

		bool even_row = true;

		for (unsigned int i = 0; i < players.size() && y >= y_increment; i++) {

			if (!even_row)
				x_offset = hex_radius;
			else
				x_offset = 0.0;

			double draw_y = y-y_increment*0.5;

			//Render the player's color
			OpenGL::Color::glColor(Render::ColorConstants::playerColor(players[i]->getTeamID()));
			OpenGL::GeometryDrawing::drawHexagon(Math::Vertex2D(x_offset+hex_radius, draw_y), hex_radius);

			OpenGL::Color::glColor(OpenGL::Color::WHITE);

			//Draw an outline if the player has finished
			if (players[i]->getPathTracker()->getFinished()) {
				glLineWidth(2.0f);
				OpenGL::GeometryDrawing::drawHexagon(Math::Vertex2D(x_offset+hex_radius, draw_y), hex_radius, true);
				glLineWidth(1.0f);
			}

			//Render the player's name
			
			glEnable(GL_TEXTURE_2D);

			entry.clear();
			entry.append(
				Render::StringTextureCache::getInstance()->getStringTexture(players[i]->getPlayerName(),
				Render::FontManager::getInstance()->getCurrentFont())
				);
			entry.render(x_offset+hex_radius*2.0, draw_y, name_height);
			y -= y_increment;

			glDisable(GL_TEXTURE_2D);

			even_row = !even_row;
		}

		
		
	}

}  // namespace HUD
}  // namespace Project
