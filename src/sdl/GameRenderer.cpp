#include <vector>

#include "GameRenderer.h"
#include "opengl/GeometryDrawing.h"
#include "opengl/MathWrapper.h"

#include "map/MapLoader.h"

#include "log/Logger.h"
#include "config.h"

namespace Project {
namespace SDL {

void GameRenderer::construct(OpenGL::Camera *camera) {
    //Instantiate the rendering objects
    meshLoader = boost::shared_ptr<Render::MeshLoader>(
        new Render::MeshLoader());
    renderer = boost::shared_ptr<Render::RenderManager>(
        new Render::RenderManager());
    lightManager = renderer->getLightManager();
    mapRenderable = boost::shared_ptr<Render::RenderList>(
        new Render::RenderList());
    
    renderer->loadShadersFile("shaders.txt");
    
    //Load the vehicle model
    Render::RenderableObject *object
        = meshLoader->loadOBJ(VEHICLE_CHASSIS_MODEL_NAME, GET_SETTING("render.model.vehicle", ""));
        
    Render::RenderableObject *objectTire
        = meshLoader->loadOBJ(VEHICLE_WHEEL_MODEL_NAME, GET_SETTING("render.model.tire", ""));
      
    object->getRenderProperties()->setTransformation(
        Math::Matrix::getScalingMatrix(GET_SETTING("render.vehicle.scale", 2.0)));
    
    objectTire->getRenderProperties()->setTransformation(
        Math::Matrix::getScalingMatrix(GET_SETTING("render.tire.scale", 2.5)));
    
    //Instantiate the map
    map = boost::shared_ptr<Map::HRMap>(new Map::HRMap());
    if (map->loadMapFile(GET_SETTING("map", "data/testtrack.hrm"))) {
        LOG(WORLD, "Loaded Map File " << GET_SETTING("map", "data/testtrack.hrm"));
    }
    else {
        LOG(WORLD, "Unable to load map " << GET_SETTING("map", "data/testtrack.hrm"));
    }
    
    //Add the map lights to the light manager
    std::vector<Render::Light*> map_lights = map->getLights();
    for (unsigned i = 0; i < map_lights.size(); i++) {
        lightManager->addLight(map_lights[i], !map_lights[i]->getHasAttenuation(), false);
    }
    
    //Set background shader
    background = boost::shared_ptr<Render::BackgroundRenderable>(
        new Render::BackgroundRenderable(camera));
    background->getRenderProperties()->setWantsShaderName("backgroundShader");
    
    minimap = boost::shared_ptr<Map::Minimap>(new Map::Minimap());
    minimap->setMapInfo(map.get());
    
    renderer->setCubeMap(map->getCubeMap());
    
    paintManager = boost::shared_ptr<Paint::PaintManager>(
        new Paint::PaintManager());
    paintManager->setPaintCells(map->getPaintCells());
    
    Map::MapLoader().load(map.get(), mapRenderable.get());
}

void GameRenderer::render(OpenGL::Camera *camera, Object::World *world) {
    
    //Init Rendering----------------------------------------------------------------------

    double near_plane = GET_SETTING("render.camera.nearplane", 0.1);
    double far_plane = GET_SETTING("render.camera.farplane", 300.0);
    double lod_threshhold = GET_SETTING("render.camera.lodthreshhold", 0.5);
    double view_length = far_plane-near_plane;
    double lod_split_plane = near_plane+view_length*lod_threshhold;

    camera->setNearPlane(near_plane);
    camera->setFarPlane(far_plane);
    camera->glProjection();

    glMatrixMode(GL_MODELVIEW);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    camera->glLookAt();

    OpenGL::Color::glColor(OpenGL::Color::WHITE);
    renderer->setCamera(camera);
  
    //Activate all lights visible to the camera
    lightManager->activateIntersectingLights(*camera->getFrustrum());

    //BG/Paint/Debug Drawing----------------------------------------------------------------------
    
    //Render the background
    background->render(renderer.get());

    //Render the paint
    paintManager->render(renderer.get());

    //Render the active lights
    if(GET_SETTING("render.drawlightspheres", false))
        lightManager->drawActiveLightSpheres(false);

    if(GET_SETTING("render.drawpathnodes", false))
        renderAIDebug();

    GLdouble clip_plane [4] = {0.0, 0.0, -1.0, -lod_split_plane};

    if (lod_threshhold < 1.0) {

        //Low Quality Rendering----------------------------------------------------------------------

        renderer->setLODLow(true);

        //Set camera culling to the further portion of the view frustrum
        camera->setNearPlane(lod_split_plane);
        camera->setFrustrumNearPlaneEnabled(true);
        camera->setFrustrumFarPlaneEnabled(false);

        //Set the clipping plane
        glLoadIdentity();
        glClipPlane(GL_CLIP_PLANE0, clip_plane);
        glEnable(GL_CLIP_PLANE0);
        camera->glLookAt();

        renderWorld(world);

    }

    //High Quality Rendering----------------------------------------------------------------------

    renderer->setLODLow(false);

    //Set camera culling to the nearer portion of the view frustrum
    camera->setNearPlane(near_plane);
    camera->setFarPlane(lod_split_plane);
    camera->setFrustrumNearPlaneEnabled(false);
    camera->setFrustrumFarPlaneEnabled(true);

    clip_plane[2] = 1.0;
    clip_plane[3] = lod_split_plane;
    //Set the clipping plane
    glLoadIdentity();
    glClipPlane(GL_CLIP_PLANE0, clip_plane);
    glEnable(GL_CLIP_PLANE0);
    camera->glLookAt();
    
    renderWorld(world);

    glDisable(GL_CLIP_PLANE0);

    //Reset the lights
    lightManager->resetLights();
}

void GameRenderer::renderWorld(Object::World *world) {
    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    
    //Render most of the world
    world->preRender();
    world->getRenderableObject()->render(renderer.get());
    
    //Render the map
    mapRenderable->render(renderer.get());
    
    //Revert the rendering state
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
}

void GameRenderer::renderAIDebug() {
    // code cloned from MapEditorWidget
    
#define MAP_EDITOR_PATHNODE_LENGTH 0.4
#define MAP_EDITOR_PATHNODE_HEIGHT 0.2
#define MAP_EDITOR_PATHNODE_FLOAT_HEIGHT 0.05
#define MAP_EDITOR_PATHNODE_COLOR OpenGL::Color::BLUE
#define MAP_EDITOR_PATHNODE_LINK_WIDTH 2.0f
#define MAP_EDITOR_PATHNODE_LINK_START_COLOR OpenGL::Color::WHITE
#define MAP_EDITOR_PATHNODE_LINK_END_COLOR OpenGL::Color::BLUE
    
    std::vector<Map::PathNode *> nodeList = map->getPathNodes();
    
    for (unsigned int i = 0; i < nodeList.size(); i++) {
        Map::PathNode* node = nodeList[i];
        
        OpenGL::Color::glColor(MAP_EDITOR_PATHNODE_COLOR);
        
        OpenGL::GeometryDrawing::drawBoundingBox3D(
            BoundingBox3D(MAP_EDITOR_PATHNODE_LENGTH,
            MAP_EDITOR_PATHNODE_HEIGHT,
            MAP_EDITOR_PATHNODE_LENGTH,
            node->getPosition()), false
            );
        
        glLineWidth(MAP_EDITOR_PATHNODE_LINK_WIDTH);
        glBegin(GL_LINES);
        const vector<Map::PathNode*>& linked_nodes = node->getNextNodes();
        for (unsigned int i = 0; i < linked_nodes.size(); i++) {
            OpenGL::Color::glColor(MAP_EDITOR_PATHNODE_LINK_START_COLOR);
            OpenGL::MathWrapper::glVertex(node->getPosition());
            OpenGL::Color::glColor(MAP_EDITOR_PATHNODE_LINK_END_COLOR);
            OpenGL::MathWrapper::glVertex(linked_nodes[i]->getPosition());
        }
        glEnd();
        glLineWidth(1.0f);
    }
}

}  // namespace SDL
}  // namespace Project
