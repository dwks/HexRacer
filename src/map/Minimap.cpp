#include "Minimap.h"
#include "render/Texture.h"
#include "render/ColorConstants.h"
#include "opengl/Color.h"
#include "opengl/GeometryDrawing.h"
#include "opengl/MathWrapper.h"

namespace Project {
namespace Map {

	Minimap::Minimap() {

		camera.setCameraType(OpenGL::Camera::ORTHOGRAPHIC);
		camera.setUpDirection(Math::Point(0.0, 0.0, 1.0));
		camera.setLookDirection(Math::Point(0.0, -1.0, 0.0));
		camera.setNearPlane(-20.0);
		camera.setFarPlane(20.0);

		trackTexture = 0;
		halfMap2DWidth = 0.0;
		halfMap2DHeight = 0.0;

	}

	Minimap::~Minimap() {

		if (trackTexture > 0)
			glDeleteTextures(1, &trackTexture);

	}

	void Minimap::setMapInfo(HRMap* map) {

		if (trackTexture > 0)
			glDeleteTextures(1, &trackTexture);

		trackTexture = Render::Texture::loadTexture2D(
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

	void Minimap::drawMinimap(double minimap_view_height, double minimap_aspect,
		Math::Point focus_pos, Object::WorldManager* world,
		Paint::PaintManager* paint_manager) {

		focus_pos.setCoord(0.0, Y_AXIS);

		camera.setAspect(minimap_aspect);
		camera.setOrthoHeight(minimap_view_height);
		camera.setPosition(Math::Point(focus_pos.getX(), 1.0f, focus_pos.getZ()));

		camera.glProjection();

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		camera.glLookAt();

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, trackTexture);
		glEnable(GL_BLEND);
		glDepthMask(GL_FALSE);
		glDisable(GL_DEPTH_TEST);
		
		//Draw the map texture
		OpenGL::Color::glColor(OpenGL::Color::WHITE, 0.5);
		glBegin(GL_QUADS);

		glTexCoord2f(1.0, 1.0);
		glVertex3f(map2DCenter.getX()-halfMap2DWidth, 0.0f, map2DCenter.getZ()-halfMap2DHeight);
		glTexCoord2f(0.0, 1.0);
		glVertex3f(map2DCenter.getX()+halfMap2DWidth, 0.0f, map2DCenter.getZ()-halfMap2DHeight);
		glTexCoord2f(0.0, 0.0);
		glVertex3f(map2DCenter.getX()+halfMap2DWidth, 0.0f, map2DCenter.getZ()+halfMap2DHeight);
		glTexCoord2f(1.0, 0.0);
		glVertex3f(map2DCenter.getX()-halfMap2DWidth, 0.0f, map2DCenter.getZ()+halfMap2DHeight);

		glEnd();

		glDisable(GL_TEXTURE_2D);

		glPointSize(3.0f);
		glBegin(GL_TRIANGLES);

		//Draw a point representing the player
		Object::WorldManager::PlayerIteratorType it
			= world->getPlayerIterator();
		while(it.hasNext()) {

			Object::Player *player = it.next();
			OpenGL::Color::glColor(Render::ColorConstants::playerColor(player->getID()));
			

			Math::Point player_pos = player->getPosition();
			Math::Point player_dir = Math::Point::point2D(player->getPhysicalObject()->getFrontDirection(), Y_AXIS).normalized()*(-1.0);
			Math::Point player_right_dir = player_dir.rotate90CW(Y_AXIS);
			double player_length = 1.5;
			double player_width = 1.0;


			OpenGL::MathWrapper::glVertex(player_pos+player_dir*player_length);
			OpenGL::MathWrapper::glVertex(player_pos+player_right_dir*player_width
				-player_dir*player_length);
			OpenGL::MathWrapper::glVertex(player_pos-player_right_dir*player_width
				-player_dir*player_length);

		}
		
		glEnd();
		glPointSize(1.0f);

		Math::Point camera_diagonal = Math::Point(camera.getOrthoWidth()*0.5, 0.0, camera.getOrthoHeight()*0.5);
		Math::BoundingBox2D minimap_box = BoundingBox2D(focus_pos-camera_diagonal, focus_pos+camera_diagonal, Y_AXIS);

		if (paint_manager) {
			//Draw the paint
			paint_manager->minimapRender(minimap_box, 0.5f);
		}

		/*
		//Draw a border around the minimap
		OpenGL::Color::glColor(OpenGL::Color::WHITE, 0.5);
		glLineWidth(3.0f);
		OpenGL::GeometryDrawing::drawBoundingBox2D(minimap_box, true);
		glLineWidth(1.0f);
		*/

		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);

	}

}  // namespace Map
}  // namespace Project
