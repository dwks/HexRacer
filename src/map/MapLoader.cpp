#include "MapLoader.h"
#include "HRMap.h"
#include "mesh/TransformedMesh.h"

#include "physics/PhysicsWorld.h"
#include "physics/PhysicsFactory.h"

#include "settings/SettingsManager.h"

#include "log/Logger.h"

namespace Project {
namespace Map {

MapLoader::~MapLoader() {
	unload();
}

void MapLoader::load(HRMap *map, Misc::ProgressTracker* progressTracker, Render::RenderList *mapRenderable, Paint::PaintManager* paintManager) {

	if (progressTracker) {
		progressTracker->setCurrentStage("Loading map collision...");
		progressTracker->setTotalSteps(Map::HRMap::NUM_MESHES);
	}
    
    //Process map meshes
    for (int i = 0; i < Map::HRMap::NUM_MESHES; i++) {

		if (progressTracker)
			progressTracker->setCurrentStep(i);

        HRMap::MeshType type = static_cast<Map::HRMap::MeshType>(i);
        if (map->getMapMesh(type)) {

            //Add visible meshes to the map renderable
            if (mapRenderable && !HRMap::meshIsInvisible(type)) {
                mapRenderable->addRenderable(map->getMapMesh(type));
            }

            //Add solid meshes to the physics
            if (HRMap::meshIsSolid(type)) {
				std::vector<Math::Triangle3D> triangles;
				map->getMapMesh(type)->appendTriangles(triangles);

                Physics::PhysicsWorld::getInstance()->registerRigidBody(
                    Physics::PhysicsFactory::createRigidTriMesh(triangles));
            }

        }
        
    }
    
    //Process mesh instances
    std::vector<Map::MeshInstance*> instances = map->getMapObjects().getMeshInstances();

	if (progressTracker) {
		progressTracker->setCurrentStage("Processing mesh instances...");
		progressTracker->setTotalSteps(static_cast<int>(instances.size()));
	}

    for (unsigned int i = 0; i < instances.size(); i++) {

		if (progressTracker)
			progressTracker->setCurrentStep(static_cast<int>(i));
        
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

			std::vector<Math::Triangle3D> triangles;
			transformed_mesh->appendTransformedTriangles(triangles);

            Physics::PhysicsWorld::getInstance()->registerRigidBody(
                Physics::PhysicsFactory::createRigidTriMesh(triangles));
        }
    }


	if (paintManager) {
		paintManager->setMap(map, progressTracker);
		LOG(WORLD, "Freeing paint info...");
		map->clearPaint();
		LOG(WORLD, "Paint info freed.");
	}
}

void MapLoader::unload() {
	for (unsigned int i = 0; i < meshTints.size(); i++) {
		delete meshTints[i];
	}
	meshTints.clear();
}

}  // namespace Map
}  // namespace Project
