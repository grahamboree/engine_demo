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
