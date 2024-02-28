#include "entity.h"

DrawData DrawData::Default;
EntityID NextEntityID = 1;

EntityState::EntityState() {
	parent.fill(EntityID_Invalid);
	positions.fill({0, 0, 0});
	rotations.fill(0);
	scales.fill({1, 1});
	pivots.fill({0, 0});
}

EntityID EntityState::CreateEntity() {
	return NextEntityID++;
}

void EntityState::UpdateLocalTransformMatrix(EntityID entity) {
	localTransform[entity] =
	       glm::translate(positions[entity]) * // position
	       glm::rotate(rotations[entity], glm::vec3{0, 0, 1}) * // rotation
	       glm::scale(glm::vec3{scales[entity], 0}) * // scale
	       glm::translate(-glm::vec3{pivots[entity], 0}); // pivot
}

void EntityState::UpdateMvpMatrix(EntityID entity, const glm::mat4& viewProjection) {
	// local transform
	MVPMatrix[entity] = localTransform[entity];

	// Apply parent transforms
	EntityID p = parent[entity];
	while (p != EntityID_Invalid) {
		MVPMatrix[entity] = localTransform[p] * MVPMatrix[entity];
		p = parent[p];
	}

	// Apply view projection
	MVPMatrix[entity] = viewProjection * MVPMatrix[entity];
}
