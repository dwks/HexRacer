#include "SubMesh.h"
#include "opengl/MathWrapper.h"
#include "misc/StdVectorFunctions.h"
#include "shader/ShaderManager.h"
#include "log/Logger.h"
#include <map>
using namespace std;
using namespace Project;
using namespace OpenGL;
using namespace Math;

namespace Project {
namespace Mesh {

	SubMesh::SubMesh() {
		paramSetter = NULL;
		triangleFanTree = NULL;
		material = NULL;
		displayList = 0;
		displayListShader =-1;
	}

	SubMesh::~SubMesh() {

		for (unsigned int i = 0; i < triangleFans.size(); i++) {
			if (triangleFans[i]->getDisplayList() > 0)
				glDeleteLists(triangleFans[i]->getDisplayList(), 1);
			delete(triangleFans[i]);
		}

		if (material != NULL)
			delete(material);

		if (triangleFanTree)
			delete(triangleFanTree);

		if (displayList > 0)
			glDeleteLists(displayList, 1);
	}


	SubMesh::SubMesh(const vector< MeshTriangle* >& _triangles, OpenGL::Material* _material, bool cullable) {

		std::map<MeshTriangle*, bool> tri_map;
		for (unsigned int i = 0; i < _triangles.size(); i++) {
			tri_map[_triangles[i]] = true;
		}

		material = _material;
		getRenderProperties()->setMaterial(material);

		triangleFanTree = NULL;

		displayList = 0;

		//Generate Triangle Fans
		while (!tri_map.empty()) {

			std::map<MeshTriangle*, bool>::iterator it = tri_map.begin();
			MeshTriangle* triangle = (*it).first;

			short center_index;
			MeshVertex* fan_center = NULL;
			//Choose the vertex with the highest degree to be the center of the triangle fan
			for (short i = 0; i < 3; i++) {
				if (fan_center == NULL || triangle->getMeshVertex(i)->degree() > fan_center->degree()) {
					fan_center = triangle->getMeshVertex(i);
					center_index = i;
				}
			}

			//Set the first three vertices of the fan
			vector<MeshVertex*> fan_vertices;
			fan_vertices.push_back(fan_center);
			fan_vertices.push_back(triangle->getMeshVertex((center_index+1)%3));
			fan_vertices.push_back(triangle->getMeshVertex((center_index+2)%3));
			//Delete the first triangle
			delete(triangle);
			tri_map.erase(it);

			bool vertex_added = true;
			//Set the rest of the fan vertices
			while (fan_center->degree() > 0 && vertex_added) {

				vertex_added = false;

				const vector<MeshTriangle*>& fan_triangles = fan_center->getAdjacentTriangles();
				MeshTriangle* fan_triangle;
				for (unsigned int i = 0; i < fan_triangles.size(); i++) {

					fan_triangle = fan_triangles[i];
					MeshVertex* next_vert = fan_triangle->getNextFanVertex(fan_center, fan_vertices[fan_vertices.size()-1]);
					if (next_vert) {
						fan_vertices.push_back(next_vert);
						delete(fan_triangle);
						tri_map.erase(fan_triangle);
						vertex_added = true;
					}

				}

			}

			triangleFans.push_back(new MeshTriangleFan(fan_vertices));
		}
		//LOG(OPENGL, "Mesh: Generated " << triangleFans.size() << " triangle fans from " << triangles.size() << " triangles.");

		if (cullable)
			generateTriangleFanTree();

	}

	void SubMesh::generateTriangleFanTree() {

		BoundingBox3D bounding_box;

		vector<ObjectSpatial*> fans;

		if (triangleFans.size() > 0) {

			for (unsigned int i = 0; i < triangleFans.size(); i++) {
				if (i == 0)
					bounding_box.setToObject(*triangleFans[0]);
				else
					bounding_box.expandToInclude(*triangleFans[i]);
				fans.push_back(triangleFans[i]);
			}

		}

		triangleFanTree = new BSPTree3D(bounding_box, TREE_SPLIT_METHOD, TREE_SPLIT_SIZE, 1.0);
		triangleFanTree->add(fans);
		//LOG(OPENGL, "Mesh: Generated BSP Tree with " << triangleFans.size() << " objects with height " << triangleFanTree->getHeight());

	}

	void SubMesh::renderGeometry(const Shader::ShaderParamSetter& setter, const Math::BoundingObject* bounding_object, const Render::RenderSettings& settings) {

		int shader_index = getRenderProperties()->getShaderIndex();

		drawingCulled = (bounding_object && triangleFanTree);
		
		genDisplayList = setter.shaderIndexActive(shader_index);
		useDisplayList = settings.getAllowDisplayLists() && (genDisplayList || !setter.getHasTangentSpace());
		

		if (!drawingCulled) {

			if (useDisplayList) {

				if (displayList > 0 && displayListShader == shader_index) {
					glCallList(displayList);
					return;
				}
				else if (genDisplayList) {
					glDeleteLists(displayList, 1);
					displayList = glGenLists(1);
					glNewList(displayList, GL_COMPILE_AND_EXECUTE);
					for (unsigned int i = 0; i < triangleFans.size(); i++) {
						drawTriangleFan(triangleFans[i], setter);
					}
					glEndList();
					displayListShader = shader_index;
					return;
				}

			}
			else {
				for (unsigned int i = 0; i < triangleFans.size(); i++) {
					drawTriangleFan(triangleFans[i], setter);
				}
			}

		}
		else {

			if (settings.getRedrawMode()) {
				for (unsigned int i = 0; i < redrawBuffer.size(); i++) {
					drawTriangleFan(redrawBuffer[i], setter);
				}
			}
			else {
				redrawBuffer.clear();
				paramSetter = &setter;
				triangleFanTree->operateQuery(*this, *bounding_object, CULLING_QUERY_TYPE);
			}
		}

	}

	inline void SubMesh::drawTriangleFan(MeshTriangleFan* fan, const Shader::ShaderParamSetter& setter) {

		bool end_list = false;

		if (drawingCulled && useDisplayList) {
			if (fan->getDisplayList() > 0 && getRenderProperties()->getShaderIndex() == fan->getShaderIndex()) {
				glCallList(fan->getDisplayList());
				return;
			}
			else if (genDisplayList) {
				glDeleteLists(fan->getDisplayList(), 1);
				fan->setDisplayList(glGenLists(1));
				glNewList(fan->getDisplayList(), GL_COMPILE_AND_EXECUTE);
				fan->setShaderIndex(getRenderProperties()->getShaderIndex());
				end_list = true;
			}
		}

		glBegin(GL_TRIANGLE_FAN);

		const vector<MeshVertex*>& vertices = fan->getVertices();
		for (unsigned int j = 0; j < vertices.size(); j++) {
			MeshVertex* vert = vertices[j];
			MathWrapper::glNormal(vert->getNormal());
			if (setter.getHasTangentSpace()) {
				setter.setStandardParamVector3(Shader::ShaderParameter::ATTRIBUTE,
					static_cast<int>(Shader::ShaderManager::AV3_TANGENT),
					vert->getTangent());
				setter.setStandardParamVector3(Shader::ShaderParameter::ATTRIBUTE,
					static_cast<int>(Shader::ShaderManager::AV3_BITANGENT),
					vert->getBitangent());
			}
			glTexCoord2d(vert->getTexCoordU(), vert->getTexCoordV());
			MathWrapper::glVertex(vert->getPosition());
		}

		glEnd();

		if (end_list)
			glEndList();

	}

	void SubMesh::operateOnObject(Math::ObjectSpatial* object) {
		MeshTriangleFan* fan = (MeshTriangleFan*)object;
		drawTriangleFan(fan, *paramSetter);
		redrawBuffer.push_back(fan);
	}

	void SubMesh::appendTriangles(std::vector<Math::Triangle3D>& vec) const {
		for (unsigned int i = 0; i < triangleFans.size(); i++) {
			const std::vector<MeshVertex*>& vertices = triangleFans[i]->getVertices();
			for (unsigned int j = 1; j < vertices.size()-1; j++) {
				vec.push_back(Triangle3D(vertices[0]->getPosition(),
					vertices[j]->getPosition(),
					vertices[j+1]->getPosition()));
			}
		}
	}
}  // namespace Mesh
}  // namespace Project
