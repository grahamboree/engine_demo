#include "mesh.h"

Mesh Mesh::DefaultQuad() {
	Mesh mesh;
	mesh.vertexData.resize(6);
	mesh.vertexData[0].pos = {0, 0, 0};
	mesh.vertexData[1].pos = {0, 1, 0};
	mesh.vertexData[2].pos = {1, 1, 0};
	mesh.vertexData[5].pos = {1, 0, 0};

	mesh.vertexData[0].norm = {0, 0, 1};
	mesh.vertexData[1].norm = {0, 0, 1};
	mesh.vertexData[2].norm = {0, 0, 1};
	mesh.vertexData[5].norm = {0, 0, 1};

	mesh.vertexData[0].uv = {0, 0};
	mesh.vertexData[1].uv = {0, 1};
	mesh.vertexData[2].uv = {1, 1};
	mesh.vertexData[5].uv = {1, 0};

	mesh.vertexData[0].color = ~0;
	mesh.vertexData[1].color = ~0;
	mesh.vertexData[2].color = ~0;
	mesh.vertexData[5].color = ~0;

	mesh.vertexData[3] = mesh.vertexData[0];
	mesh.vertexData[4] = mesh.vertexData[2];
	return mesh;
}

void Mesh::GenerateUVs() {
	// generate UVs
    const float texScale = 1.0f;
    for (int i = 0; i < vertexData.size(); i += 3) {
    	const glm::vec3& va = vertexData[i + 0].pos;
		const glm::vec3& vb = vertexData[i + 1].pos;
		const glm::vec3& vc = vertexData[i + 2].pos;

		// face normal
		// glm::vec3 norm = (data[i].norm + data[i + 1].pos + data[i + 2].pos) / 3.0f;
		glm::vec3 norm = glm::cross(vb - va, vc - va);

		int ax = 0;
		if (fabs(norm.y) > fabs(norm.x)) {
			ax = 1;
		}
		if (fabs(norm.z) > fabs(norm[ax])) {
			ax = 2;
		}
		ax = (ax + 1) % 3;
		int ay = (ax + 1) % 3;

		vertexData[i + 0].uv = glm::vec2(va[ax], va[ay]) * texScale;
		vertexData[i + 1].uv = glm::vec2(vb[ax], vb[ay]) * texScale;
		vertexData[i + 2].uv = glm::vec2(vc[ax], vc[ay]) * texScale;
    }
}
