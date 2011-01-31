#include "TransformedMesh.h"

namespace Project {
namespace Render {

	TransformedMesh::TransformedMesh(MeshGroup* mesh_group, Math::SimpleTransform transform) {

		//Set the bounding sphere of the mesh group
		meshGroupSphere = Math::BoundingSphere(Math::Point(), 0.0);
		for (int i = 0; i < 8; i++)
			meshGroupSphere.expandToInclude(mesh_group->getBoundingBox().getCorner(i));

		children.push_back(mesh_group);

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
		//return true;
		return (transformedSphere.intersects(bounding_obj));
	}

}  // namespace Render
}  // namespace Project
