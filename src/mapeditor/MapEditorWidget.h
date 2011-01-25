#ifndef MAPEDITORWIDGET_H
#define MAPEDITORWIDGET_H

#include <GL/glew.h>
#include <QtGui>
#include <QGLWidget>
#include "MouseSelectorWidget.h"
#include "opengl/Camera.h"
#include "opengl/SimpleTrackball.h"
#include "render/RenderManager.h"
#include "render/RenderList.h"
#include "render/BackgroundRenderable.h"
#include "map/HRMap.h"
using namespace Project;
using namespace OpenGL;
using namespace Math;
using namespace Map;
using namespace Render;

#define MAP_EDITOR_TRACKBALL_BUTTON Qt::LeftButton
#define CAMERA_MOVE_INC 0.25
#define CAMERA_MOVE_ORTHO_INC 0.1
#define CAMERA_MIN_ORTHO_HEIGHT 1.0
#define CAMERA_MAX_ORTHO_HEIGHT 1000.0
#define CAMERA_ORTHO_HEIGHT_INC 5.0
#define CAMERA_ORTHO_PROJECT_AXIS Y_AXIS
#define CAMERA_ORTHO_UP Point::point2D(0.0, 1.0, CAMERA_ORTHO_PROJECT_AXIS)
#define CAMERA_PERSPECTIVE_UP Point(0.0, 1.0, 0.0)
#define PRECISION_SCALE_INCREMENT 0.2f
#define PRECISION_SCALE_DRAW_SCALE 15.0f
#define MIN_PRECISION_SCALE 0.1f
#define MAX_PRECISION_SCALE 5.0f

class MapEditorWidget : public MouseSelectorWidget
{
	Q_OBJECT

private:

	int viewWidth;
	int viewHeight;
	double aspect;
	Camera* camera;
	SimpleTrackball* trackball;
	bool advancedRendering;
	bool showPaint;

	HRMap* map;
	RenderManager* renderer;
	LightManager* lightManager;
	BackgroundRenderable* background;

	double perspectiveCameraHeight;

	double precisionScale;
	double orthoHeight;

public:

	MapEditorWidget(QWidget * parent = 0, const QGLWidget * shareWidget = 0, Qt::WindowFlags f = 0);
	~MapEditorWidget();

	//QSize sizeHint() const;
	//QSize minimumSize () const;

private:

	void initializeGL();
	void paintGL();
	void resizeGL(int w, int h);

	Point screenToWorld(Point screenPos);
	void mouseClicked(Qt::MouseButton button, Point click_position, Qt::KeyboardModifiers modifiers);
	void mouseDragged(Qt::MouseButton button, Point current_position, Point click_position, Point previous_position, Qt::KeyboardModifiers modifiers);

	void updateCamera();
	void translateCamera(Point translation);
	void keyPressEvent(QKeyEvent *event);

	void mapLightsChanged();
	//void keyReleaseEvent(QKeyEvent *event);
	void wheelMoved(bool up, Qt::KeyboardModifiers modifiers);

public slots:

	void newMap();
	void loadMap(string filename);
	void saveMap();
	void saveMapAs(string filename);
	void loadMesh(HRMap::MeshType type, string filename);
	void setAdvancedRendering(bool enabled);
	void setOrthoView(bool enabled);
	void setShowPaint(bool enabled);
	void generatePaint();

};

#endif // MAPEDITORWIDGET_H
