#include "Minimap.h"
#include "opengl/TextureLoading.h"
#include "render/ColorConstants.h"
#include "shader/ShaderProgram.h"
#include "opengl/Color.h"
#include "opengl/GeometryDrawing.h"
#include "opengl/MathWrapper.h"
#include "math/BoundingCircle.h"

namespace Project {
namespace HUD {

	Minimap::Minimap() {

		camera.setCameraType(OpenGL::Camera::ORTHOGRAPHIC);
		camera.setUpDirection(Math::Point(0.0, 0.0, 1.0));
		camera.setLookDirection(Math::Point(0.0, -1.0, 0.0));
		camera.setNearPlane(-1000.0);
		camera.setFarPlane(1000.0);
		camera.setAspect(1.0);

		trackTexture = 0;
		halfMap2DWidth = 0.0;
		halfMap2DHeight = 0.0;

		alphaMaskTexture = OpenGL::TextureLoading::loadTexture2D(
			"data/hud/minimap_alpha_mask.png",
			GL_CLAMP_TO_EDGE,
			GL_CLAMP_TO_EDGE,
			GL_LINEAR,
			GL_LINEAR,
			false);

		boundingCircle = Math::BoundingCircle(Math::Point(), 0.0, Y_AXIS);
		
	}

	Minimap::~Minimap() {

		if (trackTexture > 0)
			glDeleteTextures(1, &trackTexture);
		glDeleteTextures(1, &alphaMaskTexture);

	}

	void Minimap::setMapInfo(Map::HRMap* map) {

		if (trackTexture > 0)
			glDeleteTextures(1, &trackTexture);

		trackTexture = OpenGL::TextureLoading::loadTexture2D(
			map->getMap2DFile(),
			GL_CLAMP_TO_EDGE,
			GL_CLAMP_TO_EDGE,
			GL_LINEAR,
			GL_LINEAR,
			false);

		halfMap2DWidth = map->getMap2DWidth()*0.5;
		halfMap2DHeight = map->getMap2DHeight()*0.5;
		map2DCenter = map->getMap2DCenter();

	}


	void Minimap::setDrawInfo(double minimap_view_height) {
		mapViewHeight = minimap_view_height;
		camera.setOrthoHeight(mapViewHeight);
		boundingCircle.setRadius(mapViewHeight*0.5);
	}

	void Minimap::setViewInfo(Math::Point view_position, Math::Point view_direction) {
		camera.setPosition(Math::Point::point2D(view_position, Y_AXIS));
		boundingCircle.moveCentroid(view_position);
		camera.setUpDirection(Math::Point::point2D(view_direction, Y_AXIS));
	}

	void Minimap::setWorldInfo(Object::WorldManager* _world, Paint::PaintManager* paint_manager) {
		world = _world;
		paintManager = paint_manager;
	}

	void Minimap::render(HUDRenderer* renderer) {

		camera.glProjection();

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		camera.glLookAt();

		renderer->enableAlphaMask(alphaMaskTexture);
		renderer->bindTexture(trackTexture);
		
		//Draw the map texture
		OpenGL::Color::glColor(OpenGL::Color::WHITE, 0.75);
		glBegin(GL_QUADS);

		glTexCoord2d(1.0, 1.0);
		glVertex3d(map2DCenter.getX()-halfMap2DWidth, 0.0, map2DCenter.getZ()-halfMap2DHeight);
		glTexCoord2d(0.0, 1.0);
		glVertex3d(map2DCenter.getX()+halfMap2DWidth, 0.0, map2DCenter.getZ()-halfMap2DHeight);
		glTexCoord2d(0.0, 0.0);
		glVertex3d(map2DCenter.getX()+halfMap2DWidth, 0.0, map2DCenter.getZ()+halfMap2DHeight);
		glTexCoord2d(1.0, 0.0);
		glVertex3d(map2DCenter.getX()-halfMap2DWidth, 0.0, map2DCenter.getZ()+halfMap2DHeight);

		glEnd();

		renderer->clearTexture();

		if (paintManager) {
			//Draw the paint
			paintManager->minimapRender(boundingCircle, mapViewHeight, 0.35f);
		}

		if (world) {
			//Draw the players as triangles
			Object::WorldManager::PlayerIteratorType it
				= world->getPlayerIterator();
			while(it.hasNext()) {

				Object::Player *player = it.next();

				Math::Point player_pos = Math::Point::point2D(player->getPosition(), Y_AXIS);
				Math::Point player_dir = Math::Point::point2D(player->getPhysicalObject()->getFrontDirection(), Y_AXIS).normalized();
				Math::Point player_right_dir = player_dir.rotate90CW(Y_AXIS);
				double player_length = 1.75;
				double player_width = 1.15;

				Math::Point v1 = player_pos+player_dir*player_length;
				Math::Point v2 = player_pos+player_right_dir*player_width
					-player_dir*player_length;
				Math::Point v3 = player_pos-player_right_dir*player_width
					-player_dir*player_length;
				
				OpenGL::Color player_color = Render::ColorConstants::playerColor(player->getTeamID());

				OpenGL::Color::glColor(player_color);
				glBegin(GL_TRIANGLES);
				OpenGL::MathWrapper::glVertex(v1);
				OpenGL::MathWrapper::glVertex(v2);
				OpenGL::MathWrapper::glVertex(v3);
				glEnd();

				OpenGL::Color::glColor(player_color*0.5f);
				glBegin(GL_LINE_LOOP);
				OpenGL::MathWrapper::glVertex(v1);
				OpenGL::MathWrapper::glVertex(v2);
				OpenGL::MathWrapper::glVertex(v3);
				glEnd();

			}
		}

		renderer->disableAlphaMask();

	}


}  // namespace HUD
}  // namespace Project
