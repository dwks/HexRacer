#include "MapLoader.h"
#include "HRMap.h"

#include "physics/PhysicsWorld.h"
#include "physics/PhysicsFactory.h"

#include "settings/SettingsManager.h"

#include "log/Logger.h"

namespace Project {
namespace Map {

void MapLoader::load(HRMap *map, Render::RenderList *mapRenderable) {
    //Instantiate the map
    /*map = new Map::HRMap();
    std::string mapName = GET_SETTING("map", "data/testtrack.hrm");
    if(map->loadMapFile(mapName)) {
        LOG(WORLD, "Loaded map file " << mapName);
    }
    else {
        LOG(WORLD, "Unable to load map " << mapName);
    }*/
    
    //Process map meshes
    for (int i = 0; i < Map::HRMap::NUM_MESHES; i++) {
        HRMap::MeshType type = static_cast<Map::HRMap::MeshType>(i);
        if (map->getMapMesh(type)) {

            //Add visible meshes to the map renderable
            if (mapRenderable && !HRMap::meshIsInvisible(type)) {
                mapRenderable->addRenderable(map->getMapMesh(type));
            }

            //Add solid meshes to the physics
            if (HRMap::meshIsSolid(type)) {
                Physics::PhysicsWorld::getInstance()->registerRigidBody(
                    Physics::PhysicsFactory::createRigidTriMesh(
                        map->getMapMesh(type)->getTriangles()));
            }

        }
        
    }
    
    //Process mesh instances
    std::vector<Map::MeshInstance*> instances = map->getMeshInstances();
    for (unsigned i = 0; i < instances.size(); i++) {
        
        Render::TransformedMesh* transformed_mesh = new Render::TransformedMesh(
            instances[i]->getMeshGroup(), instances[i]->getTransformation());
        
        //Add the mesh to the map renderable
        if(mapRenderable) {
            mapRenderable->addRenderable(transformed_mesh);
        }
        
        //If the instance is solid static, add it to the physics
        if (instances[i]->getType() == Map::MeshInstance::SOLID_STATIC) {
            Physics::PhysicsWorld::getInstance()->registerRigidBody(
                Physics::PhysicsFactory::createRigidTriMesh(
                    transformed_mesh->getTransformedTriangles()));
        }
    }
}

}  // namespace Map
}  // namespace Project
