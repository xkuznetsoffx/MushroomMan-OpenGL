#pragma once
#include <memory>
#include "Mesh.h"
#include "Material.h"

using SPtrMesh = std::shared_ptr<Mesh>;

class Model
{
public:
	Model(
		glm::vec3 pivotPoint,
		Material* material,
		std::vector<SPtrMesh> meshes
	);

	Model(
		glm::vec3 pivotPoint,
		Material* material,
		SPtrMesh mesh
	);

	~Model();
	void update();

	void render(Shader* shader);

	void rotate(glm::vec3 rotation);

private:
	Material* material;
	std::vector<SPtrMesh> meshes;
	glm::vec3 pivotPoint;

	void updateUniforms();
};

