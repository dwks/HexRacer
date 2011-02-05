#include "Minimap.h"
#include "render/Texture.h"
#include "render/ColorConstants.h"
#include "render/Shader.h"
#include "opengl/Color.h"
#include "opengl/GeometryDrawing.h"
#include "opengl/MathWrapper.h"

namespace Project {
namespace Map {

	Minimap::Minimap() {

		camera.setCameraType(OpenGL::Camera::ORTHOGRAPHIC);
		camera.setUpDirection(Math::Point(0.0, 0.0, 1.0));
		camera.setLookDirection(Math::Point(0.0, -1.0, 0.0));
		camera.setNearPlane(-1000.0);
		camera.setFarPlane(1000.0);

		trackTexture = 0;
		halfMap2DWidth = 0.0;
		halfMap2DHeight = 0.0;

		alphaMaskTexture = Render::Texture::loadTexture2D(
			"data/hud/minimap_alpha_mask.png",
			GL_CLAMP_TO_EDGE,
			GL_CLAMP_TO_EDGE,
			GL_LINEAR,
			GL_LINEAR,
			false);

		alphaMaskShader = new Render::Shader("shaders/alphamask.frag", "shaders/alphamask.vert");
		alphaMaskLoc = alphaMaskShader->getUniLoc("alphaMaskTexture");
		renderTextureLoc = alphaMaskShader->getUniLoc("renderTexture");
		usingTextureLoc = alphaMaskShader->getUniLoc("usingTexture");
		viewSizeLoc = alphaMaskShader->getUniLoc("viewSize");
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
		Paint::PaintManager* paint_manager, bool alpha_mask) {

		focus_pos.setCoord(0.0, Y_AXIS);

		camera.setAspect(minimap_aspect);
		camera.setOrthoHeight(minimap_view_height);
		camera.setPosition(Math::Point(focus_pos.getX(), 1.0f, focus_pos.getZ()));

		Math::Point camera_diagonal = Math::Point(camera.getOrthoWidth()*0.5, 0.0, camera.getOrthoHeight()*0.5);
		Math::BoundingBox2D minimap_box = BoundingBox2D(camera.cameraToWorld(0.0, 0.0, 0.0), camera.cameraToWorld(1.0, 1.0, 0.0), Y_AXIS);

		camera.glProjection();

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		camera.glLookAt();

		glEnable(GL_BLEND);
		glEnable(GL_TEXTURE_2D);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthMask(GL_FALSE);
		glDisable(GL_DEPTH_TEST);

		alphaMaskShader->turnShaderOn();
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, alphaMaskTexture);

		GLfloat view_size [2] = {minimap_box.widthU(), minimap_box.widthV()};
		glUniform1i(alphaMaskLoc, 1);
		glUniform1i(usingTextureLoc, 1);
		glUniform1i(renderTextureLoc, 0);
		glUniform1fv(viewSizeLoc, 2, view_size);

		glActiveTexture(GL_TEXTURE0);

		glBlendEquation(GL_FUNC_ADD);
		glColorMask(true, true, true, false);
		
		glBindTexture(GL_TEXTURE_2D, trackTexture);
		
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

		glUniform1i(usingTextureLoc, 0);

		if (paint_manager) {
			//Draw the paint
			paint_manager->minimapRender(minimap_box, 0.5f);
		}

		
		if (world) {
			//Draw the players as triangles
			Object::WorldManager::PlayerIteratorType it
				= world->getPlayerIterator();
			while(it.hasNext()) {

				Object::Player *player = it.next();

				Math::Point player_pos = Math::Point::point2D(player->getPosition(), Y_AXIS);
				Math::Point player_dir = Math::Point::point2D(player->getPhysicalObject()->getFrontDirection(), Y_AXIS).normalized()*(-1.0);
				Math::Point player_right_dir = player_dir.rotate90CW(Y_AXIS);
				double player_length = 1.75;
				double player_width = 1.15;

				Math::Point v1 = player_pos+player_dir*player_length;
				Math::Point v2 = player_pos+player_right_dir*player_width
					-player_dir*player_length;
				Math::Point v3 = player_pos-player_right_dir*player_width
					-player_dir*player_length;
				
				OpenGL::Color player_color = Render::ColorConstants::playerColor(player->getID());

				OpenGL::Color::glColor(player_color);
				glBegin(GL_TRIANGLES);
				OpenGL::MathWrapper::glVertex(v1);
				OpenGL::MathWrapper::glVertex(v2);
				OpenGL::MathWrapper::glVertex(v3);
				glEnd();

				OpenGL::Color::glColor(OpenGL::Color::WHITE, 0.5f);
				glBegin(GL_LINE_LOOP);
				OpenGL::MathWrapper::glVertex(v1);
				OpenGL::MathWrapper::glVertex(v2);
				OpenGL::MathWrapper::glVertex(v3);
				glEnd();

			}
		}

		glColorMask(true, true, true, true);
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);

		alphaMaskShader->turnShaderOff();

	}

}  // namespace Map
}  // namespace Project
