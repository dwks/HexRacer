#include "TransformedMesh.h"

namespace Project {
namespace Render {

	TransformedMesh::TransformedMesh(MeshGroup* mesh_group, Math::SimpleTransform transform) {

		meshGroup = mesh_group;

		//Set the bounding sphere of the mesh group
		meshGroupSphere = Math::BoundingSphere(Math::Point(), meshGroup->getRadiusFromOrigin());

		setTransformation(transform);
	}

	void TransformedMesh::setTransformation(Math::SimpleTransform transform) {

		transformation = transform;
		getRenderProperties()->setTransformation(transformation.getMatrix());

		//Caculate the transformed version of the mesh's bounding sphere
		transformedSphere = Math::BoundingSphere(
			meshGroupSphere.centroid()+transform.getTranslation(),
			meshGroupSphere.getRadius()*transform.getScale()
			);
	}

	bool TransformedMesh::shouldDraw( const Math::BoundingObject& bounding_obj ) {
		return (transformedSphere.intersects(bounding_obj));
	}

	vector<Math::Triangle3D> TransformedMesh::getTransformedTriangles() const {
		std::vector<Math::Triangle3D> return_triangles;
		std::vector<Math::Triangle3D> triangles = getMeshGroup()->getTriangles();
		Math::Matrix matrix = transformation.getMatrix();
		for (unsigned int i = 0; i < triangles.size(); i++) {
			return_triangles.push_back(matrix*triangles[i]);
		}
		return return_triangles;
	}

}  // namespace Render
}  // namespace Project
