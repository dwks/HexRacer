#ifndef PROJECT_RENDER__MESH_LOADER_H
#define PROJECT_RENDER__MESH_LOADER_H

#include "Mesh.h"
#include "MeshGroup.h"
#include <vector>
#include <string>
using namespace std;

namespace Project {
namespace Render {

class MeshLoader {
public:

	bool MeshLoader::loadOBJ(string model_name, string filename);
	MeshGroup* getModelByName(string model_name);
	Material* getMaterialByName(string name);
	Texture* getTextureByName(string name);


private:

	vector< MeshGroup* > models;
	vector< Material* > materials;
	vector< Texture* > textures;

};

}  // namespace Render
}  // namespace Project

#endif
