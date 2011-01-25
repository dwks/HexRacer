#include "MapEditorWidget.h"
#include "config.h"
#include "math.h"
#include "opengl/MathWrapper.h"
using namespace Paint;

MapEditorWidget::MapEditorWidget(QWidget *parent, const QGLWidget * shareWidget, Qt::WindowFlags f)
	: MouseSelectorWidget(parent, shareWidget, f)
{

	setFocusPolicy(Qt::StrongFocus);
	
	trackball = new SimpleTrackball(-1.0, 1.0);

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
	camera->glProjection();
	glViewport(0, 0, viewWidth, viewHeight);

}

void MapEditorWidget::paintGL() {

	camera->glProjection();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	Color::glColor(Color::WHITE);

	camera->glLookAt();

	if (advancedRendering) {
		//Activate all lights near the camera's focal point
		lightManager->activateNearFocalPoint(camera->getLookPosition());
		//Activate all lights visible to the camera
		lightManager->activateIntersectingLights(*camera->getFrustrum());
		//Render the active lights
		lightManager->drawActiveLightSpheres();
		glEnable(GL_LIGHTING);
	}

	renderer->setCubeMap(map->getCubeMap());
	glEnable(GL_TEXTURE_2D);

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

	lightManager->resetLights();

	/*HUD Elements************************************************************/
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glOrtho(0.0f, viewWidth, 0.0f, viewHeight, 2.0f, -2.0f);

	//Draw Precision Scale
	glEnable(GL_POINT_SMOOTH);
	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);

	glPointSize(precisionScale*PRECISION_SCALE_DRAW_SCALE);

	Color::glColor(Color(0.5f, 0.5f, 0.5f, 0.5f));
	glBegin(GL_POINTS);
	glVertex3f(viewWidth-40, viewHeight-40, 0.0f);
	glEnd();

	glPointSize(1.0f);

	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
	glDisable(GL_POINT_SMOOTH);

}

Point MapEditorWidget::screenToWorld(Point screenPos) {
	return Point(
			-((float) screenPos.getU()/(float) viewWidth*2.0f-1.0f),
			-((float) screenPos.getV()/(float) viewHeight*2.0f-1.0f)
		);
}

void MapEditorWidget::mouseClicked(Qt::MouseButton button, Point click_position, Qt::KeyboardModifiers modifiers) {

	if (button == MAP_EDITOR_TRACKBALL_BUTTON) {
		trackball->setMouseStartAt(click_position);
		//updateGL();
	}

}

void MapEditorWidget::mouseDragged(Qt::MouseButton button, Point current_position, Point click_position, Point previous_position, Qt::KeyboardModifiers modifiers) {

	Point delta = current_position-previous_position;

	if (button == MAP_EDITOR_TRACKBALL_BUTTON) {
		if (camera->getCameraType() == Camera::ORTHOGRAPHIC) {
			camera->translate(Point::point2D(delta.getU(), delta.getV(), CAMERA_ORTHO_PROJECT_AXIS)*orthoHeight*(-0.5));
			updateGL();
		}
		else {
			trackball->setMouseCurrentAt(current_position);
			updateCamera();
			updateGL();
		}
	}

}

void MapEditorWidget::wheelMoved(bool up, Qt::KeyboardModifiers modifiers) {

	if (camera->getCameraType() == Camera::ORTHOGRAPHIC) {
		if (up)
			orthoHeight -= CAMERA_ORTHO_HEIGHT_INC;
		else
			orthoHeight += CAMERA_ORTHO_HEIGHT_INC;

		orthoHeight = max(CAMERA_MIN_ORTHO_HEIGHT, orthoHeight);
		orthoHeight = min(CAMERA_MAX_ORTHO_HEIGHT, orthoHeight);

		camera->setOrthoHeight(orthoHeight);
		camera->glProjection();
		updateGL();
	}
	else {
		if (up)
			precisionScale += PRECISION_SCALE_INCREMENT;
		else
			precisionScale -= PRECISION_SCALE_INCREMENT;

		precisionScale = max(MIN_PRECISION_SCALE, precisionScale);
		precisionScale = min(MAX_PRECISION_SCALE, precisionScale);
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
	updateGL();
}

void MapEditorWidget::newMap() {
	map->clear();
	mapLightsChanged();
	updateGL();
}

void MapEditorWidget::loadMap(string filename) {
	map->loadMapFile(filename);
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

	lightManager->clear();
	vector<Light*> lights = map->getLights();
	for (int i = 0; i < lights.size(); i++) {
		lightManager->addLight(lights[i], false, true);
	}

}
void MapEditorWidget::loadMesh(HRMap::MeshType type, string filename) {
	map->loadMapMesh(type, filename);
	updateGL();
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
}

void MapEditorWidget::generatePaint() {
	map->generatePaint(0.5);
	updateGL();
}