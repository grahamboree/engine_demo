#pragma once

#include <array>
#include <vector>

#include "core.h"

// GL drawing data
struct DrawData {
	static DrawData Default;

	uint32_t vao = 0;
	uint32_t vertCount = 0;
	TextureHandle texture = 0;
	uint32_t program = 0;
};

struct EntityState
{
	static constexpr size_t MAX_ENTITY_COUNT = 2048;

	// Runtime data
	std::vector<EntityID> drawList;	
	
	// Transform
	std::array<EntityID, MAX_ENTITY_COUNT> parent {}; // Hierarchy
	std::array<glm::vec2, MAX_ENTITY_COUNT> positions {};
	std::array<float, MAX_ENTITY_COUNT> rotations {};
	std::array<glm::vec2, MAX_ENTITY_COUNT> scales {};
	std::array<glm::vec2, MAX_ENTITY_COUNT> pivots {};

	// Rendering
	std::array<DrawData, MAX_ENTITY_COUNT> drawInfo {};
	std::array<glm::mat4, MAX_ENTITY_COUNT> MVPMatrix {};
	std::array<glm::mat4, MAX_ENTITY_COUNT> localTransform {};

	EntityState();
	
	EntityID CreateEntity();

	void UpdateLocalTransformMatrix(EntityID entity);
	void UpdateMvpMatrix(EntityID entity, const glm::mat4& viewProjection);
};
