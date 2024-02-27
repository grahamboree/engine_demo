#pragma once

#include "core.h"

class Mesh {
public:
	struct VertexData {
		glm::vec3 pos;
		glm::vec2 uv;
		glm::vec3 norm;
		unsigned int color;

		bool operator==(const VertexData& other) {
			return pos == other.pos
				&& uv == other.uv
				&& norm == other.norm
				&& color == other.color;
		}
	};

	std::vector<VertexData> vertexData;

	void GenerateUVs();

	static Mesh DefaultQuad();
};
