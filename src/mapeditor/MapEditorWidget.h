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
#include "CubeMapDialog.h"
using namespace Project;
using namespace OpenGL;
using namespace Math;
using namespace Map;
using namespace Render;

class MapEditorWidget : public MouseSelectorWidget
{
	Q_OBJECT

public:

	static const int NUM_EDIT_MODES = 4;
	enum EditMode {EDIT_TRANSLATE, EDIT_ROTATE, EDIT_SCALE, EDIT_LINK};
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
	bool showInvisible;

	GLuint sphereList;
	GLuint hudCircleList;

	RenderProperties* objectBufferProperties;
	RenderProperties* normalRenderProperties;

	HRMap* map;
	RenderManager* renderer;
	LightManager* lightManager;
	BackgroundRenderable* background;

	double perspectiveCameraHeight;

	double precisionScale;
	double orthoHeight;

	MapObject::ObjectType editObjectType;
	QList<MapObject*> mapObjects[MapObject::NUM_OBJECT_TYPES];
	bool drawObjects[MapObject::NUM_OBJECT_TYPES];
	MapObject* selectedObject;
	EditMode editMode;

	int propMeshIndex;

	QImage objectBuffer;

	BSPTree3D* collisionTree;

public:

	MapEditorWidget(QWidget * parent = 0, const QGLWidget * shareWidget = 0, Qt::WindowFlags f = 0);
	~MapEditorWidget();

	Color getSelectedColor(int color_index);
	static bool editModeCompatible(MapObject::ObjectType type, EditMode mode);
	EditMode getEditMode() const { return editMode; }
	const bool* getDrawMapObject() { return drawObjects; }

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
	//void keyReleaseEvent(QKeyEvent *event);

	void wheelMoved(bool up, Qt::KeyboardModifiers modifiers);
	void addLight(Point position);
	void addPathNode(Point position);
	void addStartPoint(Point position);
	void addMeshInstance(Point position);

	void mapObjectsChanged(MapObject::ObjectType type);
	void mapCollisionChanged();

	void createObject(double u, double v);
	void selectObject(double u, double v, bool rerender = true);
	MapObject* getClickedObject(double u, double v, bool rerender = true);
	bool getRaycastPosition(double u, double v, Point& p);
	void renderObjects(MapObject::ObjectType type, bool object_buffer = false);
	static Color glBufferIndexColor(int buffer_index);

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
	void setShowInvisible(bool enabled);
	void generatePaint();
	void generatePathProgress();
	void generate2DMap(string filename);
	void setMapObjectType(MapObject::ObjectType type);
	void setDrawMapObject(bool* draw_array);
	void setEditMode(EditMode mode);
	void loadCubeMap();
	void showOptionsDialog();

	void deleteSelected();
	void deleteAll();
	void setSelectedPositionX(double x);
	void setSelectedPositionY(double y);
	void setSelectedPositionZ(double z);
	void setRotationYaw(double degrees);
	void setRotationPitch(double degrees);
	void setRotationRoll(double degrees);
	void setSelectedScale(double scale);
	void setSelectedColor(int color_index, Color color);
	void setLightStrength(double strength);
	void setLightHasAttenuation(bool has);
	void setMeshInstanceType(int type);
	void scaleAll(double scale, Point origin);

	void setPropMeshIndex(int index);
	void addPropMesh(string name, string filename);
	void removePropMesh();

signals:

	void selectedObjectChanged(MapObject*);
	void selectedPositionXChanged(double);
	void selectedPositionYChanged(double);
	void selectedPositionZChanged(double);
	void selectedRotationYawChanged(double);
	void selectedRotationPitchChanged(double);
	void selectedRotationRollChanged(double);
	void selectedScaleChanged(double);

	void propMeshAdded(string prop_mesh_name);
	void propMeshesChanged(vector<string> prop_mesh_names);

};

#endif // MAPEDITORWIDGET_H
