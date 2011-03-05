#include "MapLoader.h"
#include "HRMap.h"

#include "physics/PhysicsWorld.h"
#include "physics/PhysicsFactory.h"

#include "settings/SettingsManager.h"

#include "log/Logger.h"

namespace Project {
namespace Map {

void MapLoader::load(HRMap *map, Misc::ProgressTracker* progressTracker, Render::RenderList *mapRenderable, Paint::PaintManager* paintManager) {
    //Instantiate the map
    /*map = new Map::HRMap();
    std::string mapName = GET_SETTING("map", "data/testtrack.hrm");
    if(map->loadMapFile(mapName)) {
        LOG(WORLD, "Loaded map file " << mapName);
    }
    else {
        LOG(WORLD, "Unable to load map " << mapName);
    }*/

	mapLoadTracker = progressTracker;
	if (progressTracker) {
		progressTracker->setCurrentStage("Loading map collision...");
		progressTracker->setTotalSteps(nonPaintProgress+paintProgress);
	}
    
    //Process map meshes
    for (int i = 0; i < Map::HRMap::NUM_MESHES; i++) {

		if (progressTracker)
			progressTracker->setCurrentStep( (i/Map::HRMap::NUM_MESHES)*nonPaintProgress/2 );

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

		if (progressTracker)
			progressTracker->setCurrentStep( (i/instances.size())*(nonPaintProgress * 1.5)/2 );
        
        Mesh::TransformedMesh* transformed_mesh = new Mesh::TransformedMesh(
            instances[i]->getMeshGroup(), instances[i]->getTransformation());
        
        //Add the mesh to the map renderable
        if (mapRenderable) {

			//Load material tint
			if (instances[i]->hasTint()) {
				OpenGL::Material* tint = new OpenGL::Material("tint");

				tint->setDiffuse(instances[i]->getDiffuseTint());
				tint->setSpecular(instances[i]->getSpecularTint());
				tint->setAmbient(instances[i]->getAmbientTint());

				meshTints.push_back(tint);

				transformed_mesh->getRenderProperties()->setMaterialTint(tint);
			}

            mapRenderable->addRenderable(transformed_mesh);
        }
        
        //If the instance is solid static, add it to the physics
        if (instances[i]->getType() == Map::MeshInstance::SOLID_STATIC) {
            Physics::PhysicsWorld::getInstance()->registerRigidBody(
                Physics::PhysicsFactory::createRigidTriMesh(
                    transformed_mesh->getTransformedTriangles()));
        }
    }


	if (paintManager) {
		paintManager->setMap(map, this);
		map->clearPaint();
	}

	mapLoadTracker = NULL;
}

void MapLoader::unload() {
	for (unsigned int i = 0; i < meshTints.size(); i++) {
		delete meshTints[i];
	}
	meshTints.clear();
}

void MapLoader::setCurrentStep(int current_step) {
	if (mapLoadTracker)
		mapLoadTracker->setCurrentStep(nonPaintProgress+(current_step/totalPaintSteps)*paintProgress);
}

}  // namespace Map
}  // namespace Project
