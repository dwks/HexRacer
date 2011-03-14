#include "TransformedMesh.h"

namespace Project {
namespace Mesh {

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

	void TransformedMesh::appendTransformedTriangles(std::vector<Math::Triangle3D>& vec) const {

		std::vector<Math::Triangle3D> triangles;
		getMeshGroup()->appendTriangles(triangles);

		const Math::Matrix& matrix = transformation.getMatrix();

		for (unsigned int i = 0; i < triangles.size(); i++) {
			vec.push_back(matrix*triangles[i]);
		}
	}

}  // namespace Render
}  // namespace Project
