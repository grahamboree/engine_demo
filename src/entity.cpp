#include "entity.h"

#include "core.h"

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
	// const float rotate = rotations[entity];
	// const glm::vec2 scale = scales[entity];
	// const glm::vec2 translate = positions[entity];
	// const glm::vec2 pivot = pivots[entity];
	//
	// // Construct a TRS matrix
	// float cr = cos(rotate);
	// float sr = sin(rotate);
	//
	// float x1 = cr * scale.x;
	// float y1 = -sr;
	// float x2 = sr;
	// float y2 = cr * scale.y;
	//
	// localTransform[entity] = glm::mat4{
	// 	x1, y1, 0, x1 * pivot.x + y2 * pivot.y + translate.x,
	// 	x2, y2, 0, x2 * pivot.x + y2 * pivot.y + translate.y,
	// 	0, 0, 1, 0,
	// 	0, 0, 0, 1};

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
