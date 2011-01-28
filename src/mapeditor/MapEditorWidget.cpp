#ifdef CMAKE_BUILD
    #include "MapEditorWidget.moc"
#endif

#include "MapEditorWidget.h"
#include "config.h"
#include "opengl/MathWrapper.h"
#include "opengl/GeometryDrawing.h"
#include "math/Values.h"
#include <math.h>
#include "LightObject.h"
#include "MapEditorConstants.h"
using namespace Paint;

MapEditorWidget::MapEditorWidget(QWidget *parent, const QGLWidget * shareWidget, Qt::WindowFlags f)
	: MouseSelectorWidget(parent, shareWidget, f)
{

	setFocusPolicy(Qt::StrongFocus);
	
	trackball = new SimpleTrackball(1.0, 1.0);

	camera = new Camera(Camera::PERSPECTIVE);
	camera->setPosition(Point(0.0, 0.0, -3.0));
	camera->setLookDirection(trackball->getSpherePoint());
	camera->setFarPlane(VIEW_DISTANCE);
	camera->setFieldOfViewDegrees(60.0f);
	camera->setAspect(4.0f/3.0f);

	advancedRendering = false;
	precisionScale = 1.0;
	orthoHeight = 20.0;
	showPaint = false;

	perspectiveCameraHeight = 0.0;

	map = new HRMap();

	editObjectType = MapObject::LIGHT;
	selectedObject = NULL;
	editMode = EDIT_TRANSLATE;

	cameraMovedSinceClick = false;

	collisionTree = NULL;
}

MapEditorWidget::~MapEditorWidget() {
	delete(camera);
	delete(trackball);
	delete(background);
}

void MapEditorWidget::initializeGL() {

	glewInit();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_RESCALE_NORMAL);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glDisable(GL_LIGHTING);
	glDisable(GL_BLEND);

	GLUquadric* quadric = gluNewQuadric();
	sphereList = glGenLists(1);
	glNewList(sphereList, GL_COMPILE);
	gluSphere(quadric, MAP_EDITOR_LIGHT_SPHERE_RADIUS, 12, 12);
	glEndList();
	gluDeleteQuadric(quadric);

	hudCircleList = glGenLists(1);
	glNewList(hudCircleList, GL_COMPILE);
	int vertices = 18;
	for (int i = 0; i <= vertices; i++) {
		double angle = (double) i / (double) vertices * PI * 2.0;
		glVertex3f(cos(angle), sin(angle), 0.0f);
	}
	glEndList();

	renderer = new RenderManager();
	renderer->loadShadersFile("shaders.txt");
	lightManager = renderer->getLightManager();
	renderer->setCamera(camera);

	background = new BackgroundRenderable(camera);
	background->getRenderProperties()->setWantsShaderName("backgroundShader");

}

void MapEditorWidget::resizeGL(int w, int h) {

	viewWidth = w;
	viewHeight = h;
	aspect = (float) w / (float) h;
	camera->setAspect(aspect);
	glViewport(0, 0, viewWidth, viewHeight);

}

void MapEditorWidget::paintGL() {

	camera->glProjection();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	camera->glLookAt();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Color::glColor(Color::WHITE);

	if (advancedRendering) {
		//Activate all lights near the camera's focal point
		lightManager->activateNearFocalPoint(camera->getLookPosition());
		//Activate all lights visible to the camera
		lightManager->activateIntersectingLights(*camera->getFrustrum());
		glEnable(GL_LIGHTING);
	}

	renderer->setCubeMap(map->getCubeMap());
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_COLOR_MATERIAL);

	RenderList* rootRenderable = new RenderList();
	if (!advancedRendering) {
		rootRenderable->getRenderProperties()->setShaderOverride(true);
	}

	for (int i = 0; i < HRMap::NUM_MESHES; i++) {
		HRMap::MeshType type = static_cast<HRMap::MeshType>(i);
		if (map->getMapMesh(type)) {
			rootRenderable->addRenderable(map->getMapMesh(type));
		}
	}

	rootRenderable->render(renderer);

	delete(rootRenderable);

	if (map->getCubeMap())
		background->render(renderer);

	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);

	//Draw Paint
	if (showPaint) {
		Color::glColor(Color::YELLOW);
		vector<PaintCell*> paint_cells = map->getPaintCells();
		for (unsigned int i = 0; i < paint_cells.size(); i++) {
			PaintCell* cell = paint_cells[i];
			glBegin(GL_TRIANGLE_FAN);
			OpenGL::MathWrapper::glVertex(cell->center);
			for (int j = 0; j < Paint::PaintCell::CELL_VERTICES; j++) {
				OpenGL::MathWrapper::glVertex(cell->vertex[j]);
			}
			OpenGL::MathWrapper::glVertex(cell->vertex[0]);
			glEnd();
		}
	}

	//Draw Map Objects
	if (editObjectType != MapObject::MESH_INSTANCE) {
		renderObjects(editObjectType, false);
	}
	//Draw box around selected object
	if (selectedObject) {
		Color::glColor(SELECTED_OBJECT_BOX_COLOR);
		GeometryDrawing::drawBoundingBox3D(selectedObject->getBoundingBox(), true);
	}

	lightManager->resetLights();

	/*HUD Elements************************************************************/
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glOrtho(0.0f, viewWidth, 0.0f, viewHeight, 2.0f, -2.0f);

	glDepthMask(GL_FALSE);

	//Draw Colors

	if (selectedObject && selectedObject->hasColors()) {

		double r = MAP_EDITOR_COLOR_DISPLAY_RADIUS;

		for (int i = 0; i < 3; i++) {

			glPushMatrix();
			glTranslatef(r, viewHeight-r*(3*i+1), 0.0f);
			glScalef(r, r, r);
			glPushMatrix();
			
			Color::glColor(selectedObject->getColor(i));
			glBegin(GL_POLYGON);
			glCallList(hudCircleList);
			glEnd();
			Color::glColor(Color::WHITE);

			glBegin(GL_LINE_STRIP);
			glCallList(hudCircleList);
			glEnd();

			QString color_title;
			switch (i) {
				case 0:
					color_title = "Diffuse";
					break;
				case 1:
					color_title = "Specular";
					break;
				case 2:
					color_title = "Ambient";
					break;
			}

			glPopMatrix();
			renderText(2.5, -0.9, 0.0, color_title);
			glPopMatrix();

		}

	}

	//Draw Precision Scale Filled
	glEnable(GL_BLEND);

	Color::glColor(PRECISION_SCALE_HUD_DISC_COLOR, 0.5f);
	glPushMatrix();
	
	glTranslatef(viewWidth-40, viewHeight-40, 0.0f);
	float scale = precisionScale*PRECISION_SCALE_DRAW_SCALE;
	glScalef(scale, scale, scale);

	glBegin(GL_POLYGON);
	glCallList(hudCircleList);
	glEnd();
	glPopMatrix();

	//Draw Precision Scale Unit Outline

	Color::glColor(PRECISION_SCALE_HUD_CIRCLE_COLOR);
	
	glTranslatef(viewWidth-40, viewHeight-40, 0.0f);
	scale = PRECISION_SCALE_DRAW_SCALE;
	glScalef(scale, scale, scale);

	glBegin(GL_LINE_STRIP);
	glCallList(hudCircleList);
	glEnd();


	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
	glDisable(GL_POINT_SMOOTH);

}

Point MapEditorWidget::screenToWorld(Point screenPos) {
	return Point(
			((double) screenPos.getU()/(double) (viewWidth-1)),
			1.0-((double) screenPos.getV()/(double) (viewHeight-1))
		);
}

void MapEditorWidget::mouseClicked(Qt::MouseButton button, Point click_position, Qt::KeyboardModifiers modifiers) {

	if (button == MAP_EDITOR_MAIN_BUTTON) {
		trackball->setMouseStartAt(click_position);
	}
	else if (button == MAP_EDITOR_EDIT_BUTTON) {
		cameraMovedSinceClick = false;
	}

}

void MapEditorWidget::mouseDragged(Qt::MouseButton button, Point current_position, Point click_position, Point previous_position, Qt::KeyboardModifiers modifiers) {

	Point delta = current_position-previous_position;

	if (button == MAP_EDITOR_MAIN_BUTTON) {
		if (camera->getCameraType() == Camera::ORTHOGRAPHIC) {
			camera->translate(Point::point2D(-delta.getU(), delta.getV(), CAMERA_ORTHO_PROJECT_AXIS)*orthoHeight*(-0.5));
			updateGL();
		}
		else {
			trackball->setMouseCurrentAt(current_position);
			updateCamera();
			updateGL();
		}
	}
	else if (button == MAP_EDITOR_EDIT_BUTTON && selectedObject) {
		bool z_edit = (modifiers & MAP_EDITOR_Z_EDIT_MODIFIER);
		if (editMode == EDIT_TRANSLATE) {
			Point translation;
			if (z_edit) {
				if (camera->getCameraType() == Camera::ORTHOGRAPHIC) {
					translation = camera->getLookDirection()
						*MAP_EDITOR_Z_TRANSLATE_SCALE*delta.getV()*precisionScale;
				}
				else {
					translation = (selectedObject->getPosition()-camera->getPosition()).normalized()
						*MAP_EDITOR_Z_TRANSLATE_SCALE*delta.getV()*precisionScale;
				}
			}
			else {
				double translate_u_scale;
				double translate_v_scale;
				if (camera->getCameraType() == Camera::ORTHOGRAPHIC) {
					translate_u_scale = camera->getOrthoWidth();
					translate_v_scale = camera->getOrthoHeight();
				}
				else {
					double z = camera->getPosition().distance(selectedObject->getPosition());
					//double z = (selectedObject->getPosition()-camera->getPosition()).dotProduct(camera->getLookDirection());
					translate_u_scale = z*camera->getAspect();
					translate_v_scale = z;
				}
				translation = camera->getRightDirection()*delta.getU()*translate_u_scale*precisionScale
					+camera->getUpDirection()*delta.getV()*translate_v_scale*precisionScale;
			}
			translateSelectedObject(translation);
		}
		updateGL();
	}

}

void MapEditorWidget::mouseReleased(Qt::MouseButton button, Point release_position, Point click_position, Qt::KeyboardModifiers modifiers) {
	bool moved = cameraMovedSinceClick || (release_position.distanceSquared(click_position) >= MAP_EDITOR_NO_DRAG_CLICK_THRESHHOLD);

	if (button == MAP_EDITOR_EDIT_BUTTON) {
		if (!moved) {
			if (modifiers & MAP_EDITOR_CREATE_MODIFIER) {
				createObject(click_position.getU(), click_position.getV());
			}
			else {
				selectObject(click_position.getU(), click_position.getV());
				updateGL();
			}
		}
		else {
			if (editMode == EDIT_TRANSLATE) {
				updateSelectedObjectPosition();
			}
		}
	}
	
}
void MapEditorWidget::wheelMoved(bool up, Qt::KeyboardModifiers modifiers) {

	if (camera->getCameraType() == Camera::ORTHOGRAPHIC) {
		if (up)
			orthoHeight -= CAMERA_ORTHO_HEIGHT_INC;
		else
			orthoHeight += CAMERA_ORTHO_HEIGHT_INC;

		orthoHeight = Math::maximum<double>(CAMERA_MIN_ORTHO_HEIGHT, orthoHeight);
		orthoHeight = Math::minimum<double>(CAMERA_MAX_ORTHO_HEIGHT, orthoHeight);

		camera->setOrthoHeight(orthoHeight);
		camera->glProjection();
		updateGL();
	}
	else {
		if (up)
			precisionScale += PRECISION_SCALE_INCREMENT;
		else
			precisionScale -= PRECISION_SCALE_INCREMENT;

		precisionScale = Math::maximum<double>(MIN_PRECISION_SCALE, precisionScale);
		precisionScale = Math::minimum<double>(MAX_PRECISION_SCALE, precisionScale);
		updateGL();
	}

}
void MapEditorWidget::keyPressEvent(QKeyEvent *event) {

	switch (event->key()) {
		case Qt::Key_W:
			if (camera->getCameraType() == Camera::PERSPECTIVE)
				translateCamera(camera->getLookDirection()*CAMERA_MOVE_INC*precisionScale);
			else
				translateCamera(camera->getUpDirection()*CAMERA_MOVE_ORTHO_INC*orthoHeight);
			;break;

		case Qt::Key_A:
			if (camera->getCameraType() == Camera::PERSPECTIVE)
				translateCamera(camera->getRightDirection()*-CAMERA_MOVE_INC*precisionScale);
			else
				translateCamera(camera->getRightDirection()*-CAMERA_MOVE_ORTHO_INC*orthoHeight);
			;break;

		case Qt::Key_D:
			if (camera->getCameraType() == Camera::PERSPECTIVE)
				translateCamera(camera->getRightDirection()*CAMERA_MOVE_INC*precisionScale);
			else
				translateCamera(camera->getRightDirection()*CAMERA_MOVE_ORTHO_INC*orthoHeight);
			;break;

		case Qt::Key_S:
			if (camera->getCameraType() == Camera::PERSPECTIVE)
				translateCamera(camera->getLookDirection()*-CAMERA_MOVE_INC*precisionScale);
			else
				translateCamera(camera->getUpDirection()*-CAMERA_MOVE_ORTHO_INC*orthoHeight);
			;break;

		case Qt::Key_E:
			if (camera->getCameraType() == Camera::PERSPECTIVE)
				translateCamera(camera->getUpDirection()*CAMERA_MOVE_INC*precisionScale);
			;break;

		case Qt::Key_Q:
			if (camera->getCameraType() == Camera::PERSPECTIVE)
				translateCamera(camera->getUpDirection()*-CAMERA_MOVE_INC*precisionScale);
			;break;

	}

}

void MapEditorWidget::updateCamera() {
	camera->setLookDirection(trackball->getSpherePoint());
}

void MapEditorWidget::translateCamera(Point translation) {
	camera->translate(translation);
	if (mouse_pressed[mouseButtonToIndex(MAP_EDITOR_EDIT_BUTTON)] &&
		selectedObject && editMode == EDIT_TRANSLATE) {
		translateSelectedObject(translation);
	}
	cameraMovedSinceClick = true;
	updateGL();
}

void MapEditorWidget::newMap() {
	map->clear();
	mapLightsChanged();
	mapCollisionChanged();
	updateGL();
}

void MapEditorWidget::loadMap(string filename) {
	map->loadMapFile(filename);
	mapCollisionChanged();
	mapLightsChanged();
	updateGL();
}

void MapEditorWidget::saveMap() {
	if (map->getFilename().length() > 0) {
		map->saveMapFile(map->getFilename());
	}
}

void MapEditorWidget::saveMapAs(string filename) {

}

void MapEditorWidget::mapLightsChanged() {

	if (editObjectType == MapObject::LIGHT)
		selectedObject = NULL;

	//Delete all light objects
	int type_index = static_cast<int>(MapObject::LIGHT);
	for (unsigned int i = 0; i < mapObjects[type_index].size(); i++)
		delete(mapObjects[type_index][i]);
	mapObjects[type_index].clear();

	lightManager->clear();
	vector<Light*> lights = map->getLights();
	for (unsigned int i = 0; i < lights.size(); i++) {
		lightManager->addLight(lights[i], false, true);
		mapObjects[type_index].append(new LightObject(lights[i]));
	}


}

void MapEditorWidget::addLight(Point position) {

	Light* new_light = new Light(position);
	new_light->setStrength(20.0f);

	lightManager->addLight(new_light);
	map->addLight(new_light);

	mapObjects[static_cast<int>(MapObject::LIGHT)].append(new LightObject(new_light));
}

void MapEditorWidget::loadMesh(HRMap::MeshType type, string filename) {
	map->loadMapMesh(type, filename);
	if (type != HRMap::DECOR)
		mapCollisionChanged();
	updateGL();
}

void MapEditorWidget::clearMesh(HRMap::MeshType type) {
	map->clearMapMesh(type);
	if (type != HRMap::DECOR)
		mapCollisionChanged();
	updateGL();
}

void MapEditorWidget::mapCollisionChanged() {
	collisionTree = map->getCollisionTree();
}
void MapEditorWidget::setAdvancedRendering(bool enabled) {
	advancedRendering = enabled;
	updateGL();
}

void MapEditorWidget::setOrthoView(bool enabled) {

	if (enabled) {
		perspectiveCameraHeight = camera->getPosition().getCoord(CAMERA_ORTHO_PROJECT_AXIS);

		Point new_camera_pos = camera->getPosition();
		new_camera_pos.setCoord(camera->getFarPlane()*0.5, CAMERA_ORTHO_PROJECT_AXIS);

		Point new_camera_dir;
		new_camera_dir.setCoord(-1.0, CAMERA_ORTHO_PROJECT_AXIS);

		camera->setPosition(new_camera_pos);
		camera->setUpDirection(CAMERA_ORTHO_UP);
		camera->setLookDirection(new_camera_dir);

		camera->setOrthoHeight(orthoHeight);
		camera->setCameraType(Camera::ORTHOGRAPHIC);
	}
	else {
		Point new_camera_pos = camera->getPosition();
		new_camera_pos.setCoord(perspectiveCameraHeight, CAMERA_ORTHO_PROJECT_AXIS);

		camera->setPosition(new_camera_pos);
		camera->setUpDirection(CAMERA_PERSPECTIVE_UP);
		camera->setCameraType(Camera::PERSPECTIVE);
		updateCamera();
	}

	camera->glProjection();
	updateGL();
}

void MapEditorWidget::setShowPaint(bool enabled) {
	showPaint = enabled;
	updateGL();
}

void MapEditorWidget::generatePaint() {
	map->generatePaint(0.5);
	updateGL();
}

void MapEditorWidget::setMapObjectType(MapObject::ObjectType type) {
	if (type != editObjectType) {
		setSelectedObject(NULL);
		editObjectType = type;
		updateGL();
	}
}
void MapEditorWidget::setEditMode(EditMode mode) {
	editMode = mode;
}
void MapEditorWidget::createObject(double u, double v) {
	switch (editObjectType) {

		case MapObject::LIGHT:
			addLight(camera->cameraToWorld(u, v));
			break;

		default:
			break;

	}

	updateGL();
}
void MapEditorWidget::selectObject(double u, double v, bool rerender) {

	int type_index = static_cast<int>(editObjectType);

	if (rerender) {
		camera->glProjection();

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		camera->glLookAt();

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		renderObjects(editObjectType, true);
		objectBuffer = grabFrameBuffer();
	}

	QRgb pixel = objectBuffer.pixel(u*objectBuffer.width(), (1.0-v)*objectBuffer.height());
	int selected_index = qRed(pixel)*65536 + qGreen(pixel)*256 + qBlue(pixel);
	if (selected_index >= 0 && selected_index < mapObjects[type_index].size())
		setSelectedObject(mapObjects[type_index][selected_index]);
	else
		selectedObject = NULL;

	selectedObjectChanged(selectedObject);

}
void MapEditorWidget::renderObjects(MapObject::ObjectType type, bool object_buffer) {

	int type_index = static_cast<int>(type);

	switch (type) {
		case MapObject::LIGHT:

			for (unsigned int i = 0; i < mapObjects[type_index].size(); i++) {
				Light* light = ((LightObject*)mapObjects[type_index][i])->getLight();

				if (object_buffer)
					glBufferIndexColor(i);
				else
					Color::glColor(light->getDiffuse());
				
				glPushMatrix();
				MathWrapper::glTranslate(light->getPosition());
				glCallList(sphereList);
				glPopMatrix();
			}
			break;

		default:
			break;
	}
}

void MapEditorWidget::glBufferIndexColor(int buffer_index) {
	GLubyte r = buffer_index/65536;
	GLubyte g = (buffer_index % 65536)/256;
	GLubyte b = ((buffer_index % 65536) % 256);
	glColor3ub(r, g, b);
}
string MapEditorWidget::editModeTitle(EditMode mode) {
	switch (mode) {
		case EDIT_TRANSLATE:
			return "Translate";
		case EDIT_ROTATE:
			return "Rotate";
		case EDIT_SCALE:
			return "Scale";
		default:
			return "";
	}
}
void MapEditorWidget::deleteSelected() {
	if (selectedObject) {
		switch (editObjectType) {
			case MapObject::LIGHT:
				map->removeLight(((LightObject*)selectedObject)->getLight());
				mapLightsChanged();
				updateGL();
				return;
			default:
				return;
		}
	}
}
void MapEditorWidget::translateSelectedObject(Point translation) {
	selectedObject->translate(translation);
	
}

void MapEditorWidget::updateSelectedObjectPosition() {
	if (selectedObject) {
		Point pos = selectedObject->getPosition();
		selectedPositionXChanged(pos.getX());
		selectedPositionYChanged(pos.getY());
		selectedPositionZChanged(pos.getZ());
	}
}
void MapEditorWidget::setSelectedPositionX(double x) {
	if (selectedObject) {
		selectedObject->setPositionCoord(x, X_AXIS);
		updateGL();
	}
}
void MapEditorWidget::setSelectedPositionY(double y) {
	if (selectedObject) {
		selectedObject->setPositionCoord(y, Y_AXIS);
		updateGL();
	}
}
void MapEditorWidget::setSelectedPositionZ(double z) {
	if (selectedObject) {
		selectedObject->setPositionCoord(z, Z_AXIS);
		updateGL();
	}
}
void MapEditorWidget::setSelectedObject(MapObject* object) {
	selectedObject = object;
	if (selectedObject) {
		Point pos = selectedObject->getPosition();
		selectedPositionXChanged(pos.getX());
		selectedPositionYChanged(pos.getY());
		selectedPositionZChanged(pos.getZ());
	}
	selectedObjectChanged(object);
}
Color MapEditorWidget::getSelectedColor(int color_index) {
	if (selectedObject) {
		return selectedObject->getColor(color_index);
	}
	return Color();
}

void MapEditorWidget::setSelectedColor(int color_index, Color color) {
	if (selectedObject) {
		selectedObject->setColor(color_index, color);
	}
	updateGL();
}
void MapEditorWidget::setLightStrength(double strength) {
	if (selectedObject && editObjectType == MapObject::LIGHT) {
		Light* light = ((LightObject*)selectedObject)->getLight();
		light->setStrength(strength);
		updateGL();
	}
}
void MapEditorWidget::setLightHasAttenuation(bool has) {
	if (selectedObject && editObjectType == MapObject::LIGHT) {
		Light* light = ((LightObject*)selectedObject)->getLight();
		light->setHasAttenuation(has);
		updateGL();
	}
}