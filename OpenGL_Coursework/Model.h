#pragma once
#include <memory>
#include "Mesh.h"
#include "Material.h"

using SPtrMesh = std::shared_ptr<Mesh>;
using UPtrMesh = std::unique_ptr<Mesh>;

class Model
{
public:
	Model(
		glm::vec3 pivotPoint,
		Material* material,
		Texture* overrideTexDiff,
		Texture* overrideTexSpec,
		std::vector<SPtrMesh> meshes
	);
	~Model();
	void update();

	void render(Shader* shader);

private:
	Material* material;
	Texture* overrideTexDiff;
	Texture* overrideTexSpec;
	std::vector<SPtrMesh> meshes;
	glm::vec3 pivotPoint;

	void updateUniforms();
};

