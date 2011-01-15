#include "MeshLoader.h"
#include "model_obj.h"
#include "misc/DirectoryFunctions.h"
using namespace Project;
using namespace Math;
using namespace OpenGL;
using namespace Misc;
using namespace std;

namespace Project {
namespace Render {

	bool MeshLoader::loadOBJ(string model_name, string filename) {

		string directory = DirectoryFunctions::extractDirectory(filename);

		ModelOBJ obj_file;

		if (!obj_file.import(filename.c_str(), true))
			return false;

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
			Material* mat = getMaterialByName(obj_mat.name);
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

				materials.push_back(mat);
			}

			//Look for a pre-loaded texture with the same name
			Texture* tex = getTextureByName(obj_mat.colorMapFilename);
			if (tex == NULL && obj_mat.colorMapFilename.length() > 0) {
				//Create a new texture
				string color_map_name = string(directory).append(obj_mat.colorMapFilename);
				string normal_map_name = string(directory).append(obj_mat.bumpMapFilename);

				tex = new Texture(color_map_name, color_map_name, normal_map_name);
				textures.push_back(tex);
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

			Mesh* sub_mesh = new Mesh(face_list);
			RenderProperties* properties = new RenderProperties();
			sub_mesh->setRenderProperties(properties);
			if (mat)
				properties->setMaterial(mat);
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

		MeshGroup* new_group = new MeshGroup(model_name, mesh_list);
		models.push_back(new_group);

		return true;
		
	}

	MeshGroup* MeshLoader::getModelByName(string model_name) {
		for (unsigned int i = 0; i < models.size(); i++) {
			if (models[i]->getName() == model_name) {
				return models[i];
			}
		}
		return NULL;
	}
	Material* MeshLoader::getMaterialByName(string name) {
		for (unsigned int i = 0; i < materials.size(); i++) {
			if (materials[i]->getName() == name) {
				return materials[i];
			}
		}
		return NULL;
	}

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
