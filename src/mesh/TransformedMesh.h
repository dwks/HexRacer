#ifndef PROJECT_RENDER__TRANSFORMED_MESH_H
#define PROJECT_RENDER__TRANSFORMED_MESH_H

#include "MeshGroup.h"
#include "math/SimpleTransform.h"
#include "math/BoundingSphere.h"

namespace Project {
namespace Mesh {

class TransformedMesh
	: public Render::RenderableObject {
private:
	MeshGroup* meshGroup;
	Math::BoundingSphere meshGroupSphere;
	Math::BoundingSphere transformedSphere;
	Math::SimpleTransform transformation;
public:

	TransformedMesh(MeshGroup* mesh_group, Math::SimpleTransform transform);

	void setTransformation(Math::SimpleTransform transform);
	Math::SimpleTransform getTransformation() const { return transformation; }
	bool shouldDraw( const Math::BoundingObject& bounding_obj );
	void subRender(Render::RenderManager* manager) { meshGroup->render(manager); }
	MeshGroup* getMeshGroup() const { return meshGroup; }
	Math::BoundingSphere getBoundingSphere() const { return transformedSphere; }
	vector<Math::Triangle3D> getTransformedTriangles() const;

};

}  // namespace Render
}  // namespace Project

#endif
