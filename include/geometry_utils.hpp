#pragma once
#include <vector>
#include <glm/glm.hpp>

/*
	Position: x, y, z and normal: nx, ny, nz
	Each vertex represents a point in 3D space with a normal vector.
	The normal vector is used for lighting calculations.
*/
struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
};

// Function to calculate and update the smooth normal for a given set of vertices and indices that define triangles.
inline void computeNormals(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices) {
	for (size_t i = 0; i < vertices.size(); ++i) {
		vertices[i].normal = glm::vec3(0.0f); // Initialize normal to zero
	}
	for (size_t i = 0; i < indices.size(); i += 3) {
		unsigned int index0 = indices[i];
		unsigned int index1 = indices[i + 1];
		unsigned int index2 = indices[i + 2];
		glm::vec3 v0 = vertices[index0].position;
		glm::vec3 v1 = vertices[index1].position;
		glm::vec3 v2 = vertices[index2].position;
		glm::vec3 edge1 = v1 - v0;
		glm::vec3 edge2 = v2 - v0;
		glm::vec3 normal = glm::normalize(glm::cross(edge1, edge2));
		vertices[index0].normal += normal;
		vertices[index1].normal += normal;
		vertices[index2].normal += normal;
	}
	for (size_t i = 0; i < vertices.size(); ++i) {
		vertices[i].normal = glm::normalize(vertices[i].normal); // Normalize the normal vector
	}
}

// Function to compute triangle based indices for a grid of vertices.
inline void computeTriangleIndices(
	std::vector<unsigned int>& indices, 
	const std::vector<Vertex>& vertices,
	int height,
	int width
) {
	indices.clear();
	for (int z = 0; z < height - 1; ++z) {
		for (int x = 0; x < width - 1; ++x) {
			int topLeft = z * width + x;
			int topRight = topLeft + 1;
			int bottomLeft = (z + 1) * width + x;
			int bottomRight = bottomLeft + 1;

			// Triangle 1
			indices.push_back(topLeft);
			indices.push_back(bottomLeft);
			indices.push_back(topRight);

			// Triangle 2
			indices.push_back(topRight);
			indices.push_back(bottomLeft);
			indices.push_back(bottomRight);
		}
	}
}