#ifndef PROJECT_RENDER__MESH_LOADER_H
#define PROJECT_RENDER__MESH_LOADER_H

#include "Mesh.h"
#include "MeshGroup.h"
#include <vector>
#include <string>
using namespace std;

namespace Project {
namespace Render {

/** Loads and stores groups of meshes
*/
class MeshLoader {
private:
    static MeshLoader *instance;
public:
    static MeshLoader *getInstance() { return instance; }
public:
    MeshLoader() { instance = this; }
    ~MeshLoader();
    
	/** Load a Wavefront OBJ file to a model with name @a model_name from
		the file @a filename
		Returns a pointer to the mesh group object if successful, 0 otherwise
	*/
	MeshGroup* loadOBJ(string model_name, string filename, bool cullable = false);
	
	MeshGroup* getModelByName(string model_name, bool expectFailure = false);

	/** Delete a loaded model with the name @a model_name
		Returns true if successful
	*/
	bool deleteModelByName(string model_name, bool expectFailure = false);
	Texture* getTextureByName(string name);

private:

	vector< MeshGroup* > models;
	vector< Texture* > textures;

	bool objLoadMeshes(string filename, bool cullable, vector<Mesh*>& mesh_list, vector<MeshVertex*>& vertex_list);
	bool objLoadTriangles(string filename, vector<Math::Triangle3D>& triangles);

};

}  // namespace Render
}  // namespace Project

#endif
