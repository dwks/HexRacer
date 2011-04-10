#include "MenuBackground.h"

#include "opengl/OpenGL.h"
#include "opengl/MathWrapper.h"
#include "math/BoundingCircle.h"
#include "opengl/GeometryDrawing.h"
#include "math/Values.h"
#include <cmath>

#include "Projector.h"

namespace Project {
namespace SDL {

MenuBackground::MenuBackground()
: hexGrid(BG_HEX_RADIUS, -3.0, 3.0, -1.2, 1.2) {
    rotate = 0.0;
	bandBrightness = 0.0f;
	bandTimer = 0;
	bandSettleTimer = 0;
}

MenuBackground::~MenuBackground() {
}

void MenuBackground::render() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

	Projector::getInstance()->standardGLOrtho();
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	if (bandTimer >= 70) {


		
		if (bandSettleTimer >= 8) {
			if (bandBrightness > 1.0f)
				bandBrightness -= 0.5f;
			else
				bandBrightness = 1.0f;
		}
		else {
			bandBrightness += 0.35f;
			bandSettleTimer++;
		}
		

		Math::HexGrid::HexIndexRange range = hexGrid.hexIndexRange(Math::BoundingBox2D(6.0, 0.6, Math::Z_AXIS, Math::Point(0.0, 0.45)));
		if (hexGrid.validRange(range)) {
			for (int u = range.minUIndex; u < range.maxUIndex; u++) {
				for (int v = range.minVIndex; v < range.maxVIndex; v++) {
					Math::HexGrid::HexPosition pos = hexGrid.hexPosition(u, v);
					Math::HexGrid::HexIndex index;
					index.uIndex = u;
					index.vIndex = v;
					double brightness = (4.0f-std::fabs((float)pos.uPos*2.0f))*(bandBrightness/(std::fabs((float)pos.uPos)+1.0f));
					if (hexBrightness[index] < brightness)
						hexBrightness[index] = brightness;
				}
			}
		}

	}

	bandTimer++;

	std::map< Math::HexGrid::HexIndex, float >::iterator it = hexBrightness.begin();
	while (it != hexBrightness.end()) {
		std::map< Math::HexGrid::HexIndex, float >::iterator temp_it = it;
		++it;
		if ((*temp_it).second <= 0.0f)
			hexBrightness.erase(temp_it);
		else {
			float brightness = (*temp_it).second;

			glColor3f(0.0f, 0.05f*brightness, 0.2f*brightness);

			Math::HexGrid::HexPosition pos = hexGrid.hexPosition((*temp_it).first);
			OpenGL::GeometryDrawing::drawHexagon(Math::Point(pos.uPos, pos.vPos), BG_HEX_RADIUS*0.9, false);
			hexBrightness[(*temp_it).first] = Math::minimum(brightness-0.05f, 5.0f);
			
		}
		
	}

	/*
    rotate += 0.02;
    glRotated(rotate, 0.0, 0.0, 1.0);
    
    glColor3f(0.0f, 0.2f, 0.8f);
    
	
    glBegin(GL_TRIANGLE_FAN);
 
    OpenGL::MathWrapper::glVertex(Math::Point(-1.0, 0.0));
    OpenGL::MathWrapper::glVertex(Math::Point(0.0, 0.0));
    OpenGL::MathWrapper::glVertex(Math::Point(-0.4, 1.0));
    OpenGL::MathWrapper::glVertex(Math::Point(0.4, 1.0));
    OpenGL::MathWrapper::glVertex(Math::Point(+1.0, 0.0));
    OpenGL::MathWrapper::glVertex(Math::Point(0.4, -1.0));
    OpenGL::MathWrapper::glVertex(Math::Point(-0.4, -1.0));
    OpenGL::MathWrapper::glVertex(Math::Point(-1.0, 0.0));
    
    glEnd();
	*/
}

void MenuBackground::handleEvent(SDL_Event *event) {

	bool activate = false;
	float brightness = 0.0f;
	Math::Point mousePos;
	double radius = 0.0f;

	if (event->type == SDL_MOUSEMOTION) {

		//double aspect = Projector::getInstance()->getAspectRatio();

		mousePos = Projector::getInstance()->screenToGL(SDL::Point2D(event->button.x, event->button.y));
		activate = true;
		brightness = 0.25f;
		radius = 0.15f;

	}
	else if (event->type == SDL_MOUSEBUTTONDOWN) {

		mousePos = Projector::getInstance()->screenToGL(SDL::Point2D(event->button.x, event->button.y));
		activate = true;
		brightness = 2.0f;
		radius = 0.25f;

	}

	if (activate) {

		Math::HexGrid::HexIndexRange range = hexGrid.hexIndexRange(Math::BoundingCircle(mousePos, radius*3.0f));
		if (hexGrid.validRange(range)) {
			for (int u = range.minUIndex; u < range.maxUIndex; u++) {
				for (int v = range.minVIndex; v < range.maxVIndex; v++) {
					Math::HexGrid::HexPosition pos = hexGrid.hexPosition(u, v);
					double dist = Math::Point(pos.uPos, pos.vPos).distance(mousePos);
					if (dist <= radius) {
						Math::HexGrid::HexIndex index;
						index.uIndex = u;
						index.vIndex = v;
						hexBrightness[index] += brightness/(dist+1.0f);
					}
				}
			}
		}
	}

}

}  // namespace SDL
}  // namespace Project
