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
#include "PathNodeObject.h"
#include "StartPointObject.h"
#include "FinishPlaneObject.h"
#include "MeshInstanceObject.h"
#include "MapEditorConstants.h"
#include "CubeMapDialog.h"
#include "map/PathManager.h"
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
	showInvisible = false;

	perspectiveCameraHeight = 0.0;

	map = new HRMap();
	mapObjects[static_cast<int>(MapObject::FINISH_PLANE)].push_back(new FinishPlaneObject(map));

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
	for (int i = 0; i < MapObject::NUM_OBJECT_TYPES; i++) {
		for (int j = 0; j < mapObjects[i].size(); j++) {
			delete(mapObjects[i][j]);
		}
	}
	delete(map);
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
	renderer->getShaderManager()->loadShadersFile("shaders.txt");
	lightManager = renderer->getLightManager();
	renderer->setCamera(camera);

	background = new BackgroundRenderable(camera);
	background->getRenderProperties()->setWantsShaderName("backgroundShader");

	normalRenderProperties = new RenderProperties();
	normalRenderProperties->setShaderOverride(true);

	objectBufferProperties = new RenderProperties();
	objectBufferProperties->setShaderOverride(true);
	objectBufferProperties->setMaterialOverride(true);
	objectBufferProperties->setColorOverride(true);
	objectBufferProperties->setTexturePackOverride(true);

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

	if (!advancedRendering) {
		renderer->setRenderProperties(normalRenderProperties);
		//rootRenderable.getRenderProperties()->setShaderOverride(true);
	}

	for (int i = 0; i < HRMap::NUM_MESHES; i++) {
		HRMap::MeshType type = static_cast<HRMap::MeshType>(i);
		if (map->getMapMesh(type) && (showInvisible || !HRMap::meshIsInvisible(type))) {
			map->getMapMesh(type)->render(renderer);
		}
	}

	if (!advancedRendering)
		renderer->revertRenderProperties(normalRenderProperties);

	//Render Mesh Instances
	renderObjects(MapObject::MESH_INSTANCE, false);

	if (map->getCubeMap())
		background->render(renderer);

	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);

	//Draw Paint
	if (showPaint) {
		Color::glColor(Color::YELLOW);
		const vector<PaintCell*>& paint_cells = map->getPaintCells();
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
		MathWrapper::glMultMatrix(selectedObject->getTransformMatrix());
		GeometryDrawing::drawBoundingBox3D(selectedObject->getBoundingBox(), true);
	}

	lightManager->resetLights();

	/*HUD Elements************************************************************/
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glOrtho(0.0f, viewWidth, 0.0, viewHeight, 2.0f, -2.0f);

	glDepthMask(GL_FALSE);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_BLEND);

	Point screen_scale = Point(viewWidth, viewHeight, 0.0);

	//Draw Link Line
	if (editMode == EDIT_LINK && mouse_pressed[mouseButtonToIndex(MAP_EDITOR_EDIT_BUTTON)]) {

		glBegin(GL_LINES);
		Color::glColor(MAP_EDITOR_PATHNODE_LINK_START_COLOR);
		MathWrapper::glVertex(mouse_click_pos[mouseButtonToIndex(MAP_EDITOR_EDIT_BUTTON)].coordinateMultiply(screen_scale));
		Color::glColor(MAP_EDITOR_PATHNODE_LINK_END_COLOR);
		MathWrapper::glVertex(mouse_current_pos.coordinateMultiply(screen_scale));
		glEnd();
	}

	//Draw Colors

	if (selectedObject && selectedObject->hasColors()) {

		double r = MAP_EDITOR_COLOR_DISPLAY_RADIUS;

		glPointSize(r*2.0);

		for (int i = 0; i < 3; i++) {

			glPushMatrix();
			glTranslatef(r+2, viewHeight-r*(3*i+1)-2, 0.0f);
			glPushMatrix();
			
			Color::glColor(selectedObject->getColor(i));
			glBegin(GL_POINTS);
			glVertex3f(0.0f, 0.0f, 0.0f);
			glEnd();
			Color::glColor(Color::WHITE);

			glScalef(r, r, r);
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
			renderText(r+2.5, -0.9, 0.0, color_title);
			glPopMatrix();

		}

		glPointSize(1.0f);

	}

	//Draw Path Node Progress

	if (selectedObject && editObjectType == MapObject::PATH_NODE) {
		renderText(10.0, viewHeight-20.0, 0.0,
			QString::number(((PathNodeObject*)selectedObject)->getNode()->getProgress()));
	}

	//Draw Precision Scale Filled

	Color::glColor(PRECISION_SCALE_HUD_DISC_COLOR, 0.5f);
	float scale = precisionScale*PRECISION_SCALE_DRAW_SCALE*2.0;
	glPointSize(scale);
	glBegin(GL_POINTS);
	glVertex3f(viewWidth-40, viewHeight-40, 0.0f);
	glEnd();
	glPointSize(1.0f);

	//Draw Precision Scale Unit Outline
	Color::glColor(PRECISION_SCALE_HUD_CIRCLE_COLOR);
	
	glTranslatef(viewWidth-40, viewHeight-40, 0.0f);
	scale = PRECISION_SCALE_DRAW_SCALE;
	glScalef(scale, scale, scale);

	glBegin(GL_LINE_STRIP);
	glCallList(hudCircleList);
	glEnd();


	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glDisable(GL_LINE_SMOOTH);
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
			camera->translate(Point::point2D(-delta.getU(), delta.getV(), CAMERA_ORTHO_PROJECT_AXIS)*orthoHeight*(-1.0));
			updateGL();
		}
		else {
			trackball->setMouseCurrentAt(current_position);
			updateCamera();
			updateGL();
		}
	}
	else if (button == MAP_EDITOR_EDIT_BUTTON) {

		if (selectedObject) {

			bool z_edit = (modifiers & MAP_EDITOR_Z_EDIT_MODIFIER);

			if (editMode == EDIT_TRANSLATE) {
				/*TRANSLATION*************************************************************/
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
				
			} else if (editMode == EDIT_ROTATE) {

				double rotate_scale = MAP_EDITOR_ROTATE_SCALE*precisionScale;
				if (z_edit)
					selectedObject->rotate(delta.getV()*rotate_scale, ROLL);
				else {
					selectedObject->rotate(delta.getU()*rotate_scale, YAW);
					selectedObject->rotate(delta.getV()*rotate_scale, PITCH);
				}
			} else if (editMode == EDIT_SCALE) {
				selectedObject->addToScale(delta.getV()*MAP_EDITOR_SCALE_SCALE*precisionScale);
			}

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
			else if (modifiers & MAP_EDITOR_RAYCAST_MOVE_MODIFIER && editMode == EDIT_TRANSLATE && selectedObject) {
				Point move_pos;
				if (getRaycastPosition(click_position.getU(), click_position.getV(), move_pos)) {
					selectedObject->setPosition(move_pos);
				}
			}
			else
				selectObject(click_position.getU(), click_position.getV());

			updateGL();
		}
		else {
			if (editMode == EDIT_LINK) {

				if (editObjectType == MapObject::PATH_NODE) {
					//Linking Path Nodes
					PathNodeObject* object = (PathNodeObject*) getClickedObject(
						click_position.getU(), click_position.getV(), true
						);
					PathNodeObject* link_object = (PathNodeObject*) getClickedObject(
						release_position.getU(), release_position.getV(), false
						);

					if (object && link_object) {
						object->linkNode(link_object);
					}
					
				}
				
			}

			updateSelectedObjectPosition();
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
	mapObjectsChanged(MapObject::LIGHT);
	mapObjectsChanged(MapObject::PATH_NODE);
	mapCollisionChanged();
	updateGL();
}

void MapEditorWidget::loadMap(string filename) {
	map->loadMapFile(filename);
	for (int i = 0; i < MapObject::NUM_OBJECT_TYPES; i++) {
		MapObject::ObjectType type = static_cast<MapObject::ObjectType>(i);
		mapObjectsChanged(type);
	}
	mapCollisionChanged();
	propMeshesChanged(map->getPropMeshNames());
	updateGL();
}

void MapEditorWidget::saveMap() {
	if (map->getFilename().length() > 0) {
		map->saveMapFile(map->getFilename());
	}
}

void MapEditorWidget::saveMapAs(string filename) {
	map->saveMapFile(filename);
}

void MapEditorWidget::mapObjectsChanged(MapObject::ObjectType type) {

	if (type == MapObject::FINISH_PLANE)
		return;

	if (editObjectType == type)
		setSelectedObject(NULL);

	//Delete all map objects with the given type
	int type_index = static_cast<int>(type);
	for (int i = 0; i < mapObjects[type_index].size(); i++)
		delete(mapObjects[type_index][i]);
	mapObjects[type_index].clear();

	if (type == MapObject::LIGHT) {
		lightManager->clear();
		const vector<Light*>& lights = map->getLights();
		for (unsigned int i = 0; i < lights.size(); i++) {
			lightManager->addLight(lights[i], false, true);
			mapObjects[type_index].append(new LightObject(lights[i]));
		}
	}
	else if (type == MapObject::PATH_NODE) {
		const vector<PathNode*>& nodes = map->getPathNodes();
		for (unsigned int i = 0; i < nodes.size(); i++) {
			mapObjects[type_index].append(new PathNodeObject(nodes[i]));
		}
	}
	else if (type == MapObject::START_POINT) {
		const vector<Vertex3D*>& points = map->getStartPoints();
		for (unsigned int i = 0; i < points.size(); i++) {
			mapObjects[type_index].append(new StartPointObject(points[i]));
		}
	}
	else if (type == MapObject::MESH_INSTANCE) {
		const vector<MeshInstance*>& meshes = map->getMeshInstances();
		for (unsigned int i = 0; i < meshes.size(); i++) {
			if (meshes[i]->getMeshGroup()) {
				TransformedMesh* transformed_mesh = new TransformedMesh(meshes[i]->getMeshGroup(), meshes[i]->getTransformation());
				mapObjects[type_index].append(new MeshInstanceObject(meshes[i], transformed_mesh));
			}
		}
	}

}

void MapEditorWidget::addLight(Point position) {

	Light* new_light = new Light(position);
	new_light->setStrength(20.0f);

	lightManager->addLight(new_light);
	map->addLight(new_light);

	mapObjects[static_cast<int>(MapObject::LIGHT)].append(new LightObject(new_light));
}

void MapEditorWidget::addPathNode(Point position) {
	PathNode* new_node = new PathNode(position);
	map->addPathNode(new_node);
	mapObjects[static_cast<int>(MapObject::PATH_NODE)].append(new PathNodeObject(new_node));
}
void MapEditorWidget::addStartPoint(Point position) {
	Vertex3D* new_point = new PathNode(position);
	map->addStartPoint(new_point);
	mapObjects[static_cast<int>(MapObject::START_POINT)].append(new StartPointObject(new_point));
}
void MapEditorWidget::addMeshInstance(Point position) {

	if (propMeshIndex >= 0) {

		MeshInstance* instance = new MeshInstance(map->getPropMeshName(propMeshIndex), SimpleTransform(position));
		if (instance->getMeshGroup() && map->addMeshInstance(instance)) {
			TransformedMesh* transformed_mesh = new TransformedMesh(instance->getMeshGroup(), instance->getTransformation());
			mapObjects[static_cast<int>(MapObject::MESH_INSTANCE)].append(new MeshInstanceObject(instance, transformed_mesh));
		}
		else {
			delete(instance);
		}
	}

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

		bool camera_has_focus = false;
		Point camera_focus = Point();
		if (selectedObject) {
			camera_focus = selectedObject->getPosition();
			camera_has_focus = true;
		} else if (getRaycastPosition(0.5, 0.5, camera_focus)) {
			camera_has_focus = true;
		}
		
		if (camera_has_focus) {
			camera->setPosition(camera_focus+CAMERA_DISABLE_ORTHO_OFFSET);
			trackball->setLongitude(PI*0.5);
			trackball->setLatitude(-PI*0.15);
			//camera->setLookPosition(camera_focus);
		}
		else {
			Point new_camera_pos = camera->getPosition();
			new_camera_pos.setCoord(perspectiveCameraHeight, CAMERA_ORTHO_PROJECT_AXIS);
			camera->setLookPosition(new_camera_pos);
		}

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

void MapEditorWidget::setShowInvisible(bool enabled) {
	showInvisible = enabled;
	updateGL();
}
void MapEditorWidget::generatePaint() {
	map->generatePaint();
	updateGL();
}

void MapEditorWidget::generatePathProgress() {

	const vector<PathNode*>& path_nodes = map->getPathNodes();
	if (path_nodes.empty())
		return;

	//Find the path node nearest to the finish plane
	Point finish_point = map->getFinishPlane().centroid();
	PathNode* nearest_node = NULL;
	double min_distance_squared = 0.0;

	for (unsigned int i = 0; i < path_nodes.size(); i++) {
		double dist_squared = path_nodes[i]->getPosition().distanceSquared(finish_point);
		if (nearest_node == NULL || dist_squared < min_distance_squared) {
			nearest_node = path_nodes[i];
			min_distance_squared = dist_squared;
		}
	}

	//Set the start node's progress to 0, set the node previous to it to 1.0
	nearest_node->setProgress(0.0f);
	const vector<PathNode*>& previous_nodes = nearest_node->getPreviousNodes();
	if (previous_nodes.empty())
		return;

	previous_nodes[0]->setProgress(1.0f);

	//Run the pathing algorithm on the path
	PathManager::calculatePathProgress(nearest_node, previous_nodes[0]);

}
void MapEditorWidget::generate2DMap(string filename) {

	MeshGroup* track_mesh = map->getMapMesh(HRMap::TRACK);
	MeshGroup* invis_track_mesh = map->getMapMesh(HRMap::INVIS_TRACK);

	if (track_mesh || invis_track_mesh) {

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		camera->glProjection();

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		camera->glLookAt();

		Color::glColor(Color::WHITE);

		RenderList render_list;
		render_list.getRenderProperties()->setTransformation(Matrix());
		render_list.getRenderProperties()->setColorOverride(true);
		render_list.getRenderProperties()->setTexturePackOverride(true);
		render_list.getRenderProperties()->setMaterialOverride(true);
		render_list.getRenderProperties()->setShaderOverride(true);

		if (track_mesh)
			render_list.addRenderable(track_mesh);
		if (invis_track_mesh)
			render_list.addRenderable(invis_track_mesh);
		render_list.render(renderer);

		QImage map_image = grabFrameBuffer();
		map_image.save(filename.c_str());

		map->setMap2DFile(filename);
		map->setMap2DCenter(Point::point2D(camera->getPosition(), Y_AXIS));
		map->setMap2DWidth(camera->getOrthoWidth());
		map->setMap2DHeight(camera->getOrthoHeight());

		updateGL();

	}

}
void MapEditorWidget::setMapObjectType(MapObject::ObjectType type) {
	if (type != editObjectType) {
		setSelectedObject(NULL);
		editObjectType = type;
		if (editObjectType == MapObject::FINISH_PLANE)
			setSelectedObject(mapObjects[static_cast<int>(MapObject::FINISH_PLANE)][0]);
		updateGL();
	}
}
void MapEditorWidget::setEditMode(EditMode mode) {
	editMode = mode;
}
void MapEditorWidget::createObject(double u, double v) {

	Point create_pos = Point();

	switch (editObjectType) {

		case MapObject::LIGHT:
			addLight(camera->cameraToWorld(u, v));
			break;

		case MapObject::PATH_NODE:
			if (getRaycastPosition(u, v, create_pos)) {
				create_pos.setY(create_pos.getY()+MAP_EDITOR_PATHNODE_FLOAT_HEIGHT);
				addPathNode(create_pos);
			}
			else {
				addPathNode(camera->cameraToWorld(u, v));
			}
			break;

		case MapObject::START_POINT:
			if (getRaycastPosition(u, v, create_pos)) {
				create_pos.setY(create_pos.getY()+MAP_EDITOR_STARTPOINT_FLOAT_HEIGHT);
				addStartPoint(create_pos);
			}
			break;

		case MapObject::FINISH_PLANE:
			if (getRaycastPosition(u, v, create_pos)) {
				map->getFinishPlane().moveCentroid(create_pos);
			}
			break;

		case MapObject::MESH_INSTANCE:
			addMeshInstance(camera->cameraToWorld(u, v, 3.0));
			break;

		default:
			break;

	}

	updateGL();
}
void MapEditorWidget::selectObject(double u, double v, bool rerender) {

	if (editObjectType != MapObject::FINISH_PLANE) {
		setSelectedObject(getClickedObject(u, v, rerender));
	}
}
MapObject* MapEditorWidget::getClickedObject(double u, double v, bool rerender) {

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
	int click_index = qRed(pixel)*65536 + qGreen(pixel)*256 + qBlue(pixel);
	if (click_index >= 0 && click_index < mapObjects[type_index].size())
		return (mapObjects[type_index][click_index]);
	else
		return NULL;

}
void MapEditorWidget::renderObjects(MapObject::ObjectType type, bool object_buffer) {

	int type_index = static_cast<int>(type);

	switch (type) {
		case MapObject::LIGHT:

			for (int i = 0; i < mapObjects[type_index].size(); i++) {
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
		
		case MapObject::PATH_NODE:

			for (int i = 0; i < mapObjects[type_index].size(); i++) {
				PathNode* node = ((PathNodeObject*)mapObjects[type_index][i])->getNode();

				if (object_buffer)
					glBufferIndexColor(i);
				else
					Color::glColor(MAP_EDITOR_PATHNODE_COLOR);

				GeometryDrawing::drawBoundingBox3D(
					BoundingBox3D(MAP_EDITOR_PATHNODE_LENGTH,
					MAP_EDITOR_PATHNODE_HEIGHT,
					MAP_EDITOR_PATHNODE_LENGTH,
					node->getPosition()), false
					);

				if (!object_buffer) {
					glLineWidth(MAP_EDITOR_PATHNODE_LINK_WIDTH);
					glBegin(GL_LINES);
					const vector<PathNode*>& linked_nodes = node->getNextNodes();
					for (unsigned int i = 0; i < linked_nodes.size(); i++) {
						Color::glColor(MAP_EDITOR_PATHNODE_LINK_START_COLOR);
						MathWrapper::glVertex(node->getPosition());
						Color::glColor(MAP_EDITOR_PATHNODE_LINK_END_COLOR);
						MathWrapper::glVertex(linked_nodes[i]->getPosition());
					}
					glEnd();
					glLineWidth(1.0f);
				}
			}
			break;

		case MapObject::START_POINT:

			for (int i = 0; i < mapObjects[type_index].size(); i++) {
				StartPointObject* point_object = ((StartPointObject*)mapObjects[type_index][i]);

				if (object_buffer)
					glBufferIndexColor(i);
				else
					Color::glColor(MAP_EDITOR_STARTPOINT_COLOR);

				GeometryDrawing::drawBoundingBox3D(
					BoundingBox3D(MAP_EDITOR_STARTPOINT_LENGTH,
					MAP_EDITOR_STARTPOINT_HEIGHT,
					MAP_EDITOR_STARTPOINT_LENGTH,
					point_object->getPosition()), false
					);
			}
			break;

		case MapObject::FINISH_PLANE:

			if (!object_buffer) {
				Color::glColor(MAP_EDITOR_FINISH_PLANE_COLOR);
				GeometryDrawing::drawBoundingPlane3D(map->getFinishPlane(), true);
			}
			break;

		case MapObject::MESH_INSTANCE:

			if (object_buffer)
				renderer->setRenderProperties(objectBufferProperties);
			else if (!advancedRendering)
				renderer->setRenderProperties(normalRenderProperties);
			
			for (int i = 0; i < mapObjects[type_index].size(); i++) {
			
				if (object_buffer)
					glBufferIndexColor(i);

				TransformedMesh* mesh_instance = ((MeshInstanceObject*)mapObjects[type_index][i])->getTransformedMesh();
				mesh_instance->render(renderer);
			}

			if (object_buffer)
				renderer->revertRenderProperties(objectBufferProperties);
			else if (!advancedRendering)
				renderer->revertRenderProperties(normalRenderProperties);

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
		case EDIT_LINK:
			return "Link";
		default:
			return "";
	}
}
bool MapEditorWidget::editModeCompatible(MapObject::ObjectType type, EditMode mode) {
	switch (type) {

		case MapObject::LIGHT:
			switch (mode) {
				case EDIT_TRANSLATE:
					return true;
				default:
					return false;
			}

		case MapObject::PATH_NODE:
			switch (mode) {
				case EDIT_TRANSLATE: case EDIT_LINK:
					return true;
				default:
					return false;
			}

		case MapObject::START_POINT:
			switch (mode) {
				case EDIT_TRANSLATE:
					return true;
				default:
					return false;
			}

		case MapObject::MESH_INSTANCE:
			switch (mode) {
				case EDIT_TRANSLATE: case EDIT_SCALE: case EDIT_ROTATE:
					return true;
				default:
					return false;
			}

		case MapObject::FINISH_PLANE:
			switch (mode) {
				case EDIT_TRANSLATE: case EDIT_ROTATE:
					return true;
				default:
					return false;
			}


		default:
			return false;
	}
}
void MapEditorWidget::deleteSelected() {
	if (selectedObject && editObjectType != MapObject::FINISH_PLANE) {

		switch (editObjectType) {
			case MapObject::LIGHT:
				map->removeLight(((LightObject*)selectedObject)->getLight());
				break;
			case MapObject::PATH_NODE:
				map->removePathNode(((PathNodeObject*)selectedObject)->getNode());
				break;
			case MapObject::START_POINT:
				map->removeStartPoint(((StartPointObject*)selectedObject)->getStartPoint());
				break;
			case MapObject::MESH_INSTANCE:
				map->removeMeshInstance(((MeshInstanceObject*)selectedObject)->getMeshInstance());
				break;
			default:
				return;
		}

		mapObjectsChanged(editObjectType);
		updateGL();
	}
}
void MapEditorWidget::deleteAll() {
	if (editObjectType != MapObject::FINISH_PLANE) {

		switch (editObjectType) {
			case MapObject::LIGHT:
				map->clearLights();
				break;
			case MapObject::PATH_NODE:
				map->clearPathNodes();
				break;
			case MapObject::START_POINT:
				map->clearStartPoints();
				break;
			case MapObject::MESH_INSTANCE:
				map->clearMeshInstances();
				break;
			default:
				return;
		}

		mapObjectsChanged(editObjectType);
		updateGL();
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

		selectedRotationYawChanged(radiansToDegrees(selectedObject->getRotation(YAW)));
		selectedRotationPitchChanged(radiansToDegrees(selectedObject->getRotation(PITCH)));
		selectedRotationRollChanged(radiansToDegrees(selectedObject->getRotation(ROLL)));

		selectedScaleChanged(selectedObject->getScale());
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
void MapEditorWidget::setRotationYaw(double degrees) {
	if (selectedObject) {
		selectedObject->setRotation(degreesToRadians(degrees), YAW);
		updateGL();
	}
}
void MapEditorWidget::setRotationPitch(double degrees) {
	if (selectedObject) {
		selectedObject->setRotation(degreesToRadians(degrees), PITCH);
		updateGL();
	}
}
void MapEditorWidget::setRotationRoll(double degrees) {
	if (selectedObject) {
		selectedObject->setRotation(degreesToRadians(degrees), ROLL);
		updateGL();
	}
}
void MapEditorWidget::setSelectedScale(double scale) {
	if (selectedObject) {
		selectedObject->setScale(scale);
		updateGL();
	}
}
void MapEditorWidget::setSelectedObject(MapObject* object) {
	selectedObject = object;
	selectedObjectChanged(object);
	updateSelectedObjectPosition();
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
void MapEditorWidget::setMeshInstanceType(int type) {
	if (selectedObject && editObjectType == MapObject::MESH_INSTANCE) {
		MeshInstance* instance = ((MeshInstanceObject*)selectedObject)->getMeshInstance();
		instance->setType(static_cast<MeshInstance::InstanceType>(type));
	}
}
bool MapEditorWidget::getRaycastPosition(double u, double v, Point& p) {
	if (collisionTree) {
		Ray ray = camera->cameraRay(u, v);
		RayIntersection intersect = collisionTree->rayIntersection(ray);
		p = ray.atT(intersect.t);
		return intersect.intersects;
	}
	return false;
}
void MapEditorWidget::loadCubeMap() {
	map->setCubeMapFile( CubeMapDialog::showCubeMapDialog( *map->getCubeMapFile(), this) );
	updateGL();
}

void MapEditorWidget::setPropMeshIndex(int index) {
	propMeshIndex = index;
}

void MapEditorWidget::addPropMesh(string name, string filename) {
	if (map->addPropMesh(name, filename)) {
		propMeshAdded(name);
	}
}
void MapEditorWidget::removePropMesh() {
	if (map->removePropMesh(propMeshIndex)) {
		mapObjectsChanged(MapObject::MESH_INSTANCE);
		propMeshesChanged(map->getPropMeshNames());
		updateGL();
	}
}