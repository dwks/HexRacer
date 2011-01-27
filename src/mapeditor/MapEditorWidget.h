#ifndef MAPEDITORWIDGET_H
#define MAPEDITORWIDGET_H

#include <GL/glew.h>
#include <QtGui>
#include <QGLWidget>
#include "MouseSelectorWidget.h"
#include "MapObject.h"
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

class MapEditorWidget : public MouseSelectorWidget
{
	Q_OBJECT

public:

	static const int NUM_EDIT_MODES = 3;
	enum EditMode {EDIT_TRANSLATE, EDIT_ROTATE, EDIT_SCALE};
	static string editModeTitle(EditMode mode);

private:

	int viewWidth;
	int viewHeight;
	double aspect;
	Camera* camera;
	SimpleTrackball* trackball;
	bool advancedRendering;
	bool showPaint;
	bool cameraMovedSinceClick;

	GLuint sphereList;
	GLuint hudCircleList;

	HRMap* map;
	RenderManager* renderer;
	LightManager* lightManager;
	BackgroundRenderable* background;

	double perspectiveCameraHeight;

	double precisionScale;
	double orthoHeight;

	MapObject::ObjectType editObjectType;
	QList<MapObject*> mapObjects[MapObject::NUM_OBJECT_TYPES];
	MapObject* selectedObject;
	EditMode editMode;

	QImage objectBuffer;

	BSPTree3D* collisionTree;

public:

	MapEditorWidget(QWidget * parent = 0, const QGLWidget * shareWidget = 0, Qt::WindowFlags f = 0);
	~MapEditorWidget();

	Color getSelectedColor(int color_index);

	//QSize sizeHint() const;
	//QSize minimumSize () const;

private:

	void initializeGL();
	void paintGL();
	void resizeGL(int w, int h);

	Point screenToWorld(Point screenPos);
	void mouseClicked(Qt::MouseButton button, Point click_position, Qt::KeyboardModifiers modifiers);
	void mouseDragged(Qt::MouseButton button, Point current_position, Point click_position, Point previous_position, Qt::KeyboardModifiers modifiers);
	void mouseReleased(Qt::MouseButton button, Point release_position, Point click_position, Qt::KeyboardModifiers modifiers);

	void updateCamera();
	void translateCamera(Point translation);
	void keyPressEvent(QKeyEvent *event);

	void mapLightsChanged();
	void mapCollisionChanged();
	//void keyReleaseEvent(QKeyEvent *event);
	void wheelMoved(bool up, Qt::KeyboardModifiers modifiers);
	void addLight(Point position);

	void createObject(double u, double v);
	void selectObject(double u, double v, bool rerender = true);
	void renderObjects(MapObject::ObjectType type, bool object_buffer = false);
	static void glBufferIndexColor(int buffer_index);

	void translateSelectedObject(Point translation);
	void setSelectedObject(MapObject* object);
	void updateSelectedObjectPosition();

public slots:

	void newMap();
	void loadMap(string filename);
	void saveMap();
	void saveMapAs(string filename);
	void loadMesh(HRMap::MeshType type, string filename);
	void clearMesh(HRMap::MeshType type);
	void setAdvancedRendering(bool enabled);
	void setOrthoView(bool enabled);
	void setShowPaint(bool enabled);
	void generatePaint();
	void setMapObjectType(MapObject::ObjectType type);
	void setEditMode(EditMode mode);

	void deleteSelected();
	void setSelectedPositionX(double x);
	void setSelectedPositionY(double y);
	void setSelectedPositionZ(double z);
	void setSelectedColor(int color_index, Color color);
	void setLightStrength(double strength);
	void setLightHasAttenuation(bool has);

signals:

	void selectedObjectChanged(MapObject*);
	void selectedPositionXChanged(double);
	void selectedPositionYChanged(double);
	void selectedPositionZChanged(double);

};

#endif // MAPEDITORWIDGET_H
