#include "MeshLoader.h"
#include "model_obj.h"
#include "misc/DirectoryFunctions.h"
#include "misc/StdVectorFunctions.h"
#include "log/Logger.h"
using namespace Project;
using namespace Math;
using namespace OpenGL;
using namespace Misc;
using namespace std;

namespace Project {
namespace Render {

    MeshLoader *MeshLoader::instance = 0;

    MeshLoader::~MeshLoader() {
        for(vector< MeshGroup* >::size_type x = 0; x < models.size(); x ++) {
            delete models[x];
        }
        
        for(vector< Texture* >::size_type x = 0; x < textures.size(); x ++) {
            delete textures[x];
        }
    }
    
	MeshGroup* MeshLoader::loadOBJ(string model_name, string filename) {

		//Check if a model with the same name already exists
		MeshGroup* group = getModelByName(model_name, true);
		if (group)
			return group;

		string directory = DirectoryFunctions::extractDirectory(filename);

		ModelOBJ obj_file;

		if (!obj_file.import(filename.c_str(), true)) {
            LOG(OPENGL, "Can't open .OBJ file \"" << filename << "\"");
			return NULL;
        }

		vector<MeshVertex*> vert_list;
		vector<Mesh*> mesh_list;

		//Load Vertices
		for (int i = 0; i < obj_file.getNumberOfVertices(); i++) {

			ModelOBJ::Vertex vertex = obj_file.getVertex(i);
			vert_list.push_back(new MeshVertex(
				Point(vertex.position[0], vertex.position[1], vertex.position[2]),
				Point(vertex.normal[0], vertex.normal[1], vertex.normal[2]),
				Point(vertex.tangent[0], vertex.tangent[1], vertex.tangent[2]),
				Point(vertex.bitangent[0], vertex.bitangent[1], vertex.bitangent[2]),
				vertex.texCoord[0],
				1.0f-vertex.texCoord[1]));

		}

		const int* index = obj_file.getIndexBuffer();

		//Load Meshes
		for (int i = 0; i < obj_file.getNumberOfMeshes(); i++) {

			ModelOBJ::Mesh mesh = obj_file.getMesh(i);

			ModelOBJ::Material obj_mat = *mesh.pMaterial;

			//Look for a pre-loaded material with the same name
			//Material* mat = getMaterialByName(obj_mat.name);
			Material* mat = NULL;
			if (mat == NULL && obj_mat.name.length() > 0) {
				//Create a new material
				mat = new Material(obj_mat.name);

				mat->setDiffuse(
					Color(obj_mat.diffuse[0], obj_mat.diffuse[1], obj_mat.diffuse[2], obj_mat.diffuse[3])
				);
				mat->setSpecular(
					Color(obj_mat.specular[0], obj_mat.specular[1], obj_mat.specular[2], obj_mat.specular[3])
				);
				mat->setAmbient(
					Color(obj_mat.ambient[0], obj_mat.ambient[1], obj_mat.ambient[2], obj_mat.ambient[3])
				);
				mat->setShininess(obj_mat.shininess*12.0f+1.0f);
			}

			bool hasColorMap = (obj_mat.colorMapFilename.length() > 0);
			bool hasNormalMap = (obj_mat.bumpMapFilename.length() > 0);
			bool hasGlowMap = (obj_mat.glowMapFilename.length() > 0);

			string tex_name;
			if (hasColorMap)
				tex_name = obj_mat.colorMapFilename;
			else {
				if (hasNormalMap) {
					tex_name = obj_mat.bumpMapFilename;
				}
				else if (hasGlowMap) {
					tex_name = obj_mat.glowMapFilename;
				}
			}

			Texture* tex = NULL;

			if (hasColorMap || hasNormalMap || hasGlowMap) {

				//Look for a pre-loaded texture with the same name
				tex = getTextureByName(tex_name);
				if (tex == NULL && mat != NULL) {
					//Create a new texture
					string color_map_name = DirectoryFunctions::fromRelativeFilename(directory, obj_mat.colorMapFilename);
					string normal_map_name = DirectoryFunctions::fromRelativeFilename(directory, obj_mat.bumpMapFilename);
					string glow_map_name = DirectoryFunctions::fromRelativeFilename(directory, obj_mat.glowMapFilename);

					tex = new Texture(tex_name, color_map_name, normal_map_name, glow_map_name);
					textures.push_back(tex);
				}
			}

			vector<MeshTriangle*> face_list;

			int vert_index = mesh.startIndex;
			for (int j = 0; j < mesh.triangleCount; j++) {
				MeshTriangle* tri = new MeshTriangle(
					vert_list[index[vert_index]],
					vert_list[index[vert_index+1]],
					vert_list[index[vert_index+2]]);
				face_list.push_back(tri);
				vert_index += 3;
			}

			Mesh* sub_mesh = new Mesh(face_list, mat);
			RenderProperties* properties = sub_mesh->getRenderProperties();
			if (mat) {
				properties->setMaterial(mat);
				properties->setWantsShaderName(obj_mat.shaderName);
			}
			if (tex)
				properties->setTexture(tex);

			mesh_list.push_back(sub_mesh);
		}

		/*
		float width = obj_file.getWidth()*0.5f;
		float height = obj_file.getHeight()*0.5f;
		float length = obj_file.getLength()*0.5f;
		obj_file.destroy();
		*/

		MeshGroup* new_group = new MeshGroup(model_name, mesh_list, vert_list);
		models.push_back(new_group);

		obj_file.destroy(); //Unload the obj file content

		return new_group;
		
	}

	MeshGroup* MeshLoader::getModelByName(string model_name, bool expectFailure) {
		for (unsigned int i = 0; i < models.size(); i++) {
			if (models[i]->getName() == model_name) {
				return models[i];
			}
		}
		if(!expectFailure) {
            LOG(OPENGL, model_name.append(": Model Not Found"));
        }
		return NULL;
	}

	bool MeshLoader::deleteModelByName(string model_name) {
		for (unsigned int i = 0; i < models.size(); i++) {
			if (models[i]->getName() == model_name) {
				delete(models[i]);
				Misc::vectorRemoveAtIndex(models, i);
				return true;
			}
		}
		LOG(OPENGL, model_name.append(": Model Not Found"));
		return false;
	}
	/*
	Material* MeshLoader::getMaterialByName(string name) {
		for (unsigned int i = 0; i < materials.size(); i++) {
			if (materials[i]->getName() == name) {
				return materials[i];
			}
		}
		return NULL;
	}
	*/

	Texture* MeshLoader::getTextureByName(string name) {
		for (unsigned int i = 0; i < textures.size(); i++) {
			if (textures[i]->getName() == name) {
				return textures[i];
			}
		}
		return NULL;
	}


}  // namespace Render
}  // namespace Project
