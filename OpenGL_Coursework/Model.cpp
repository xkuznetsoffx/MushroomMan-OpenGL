#include "Model.h"

Model::Model(
	glm::vec3 pivotPoint,
	Material* material,
	Texture* overrideTexDiff,
	Texture* overrideTexSpec,
	std::vector<SPtrMesh> meshes
)
	:
	pivotPoint(pivotPoint), material(material), 
	overrideTexDiff(overrideTexDiff),
	overrideTexSpec(overrideTexSpec)
{
	this->meshes.reserve(meshes.size());
	for (const auto& mesh : meshes) {
		this->meshes.emplace_back
		(
			std::make_shared<Mesh>(*mesh)
		);
	}
		
}

Model::Model(
	glm::vec3 pivotPoint,
	Material* material,
	Texture* overrideTexDiff,
	Texture* overrideTexSpec,
	SPtrMesh mesh
)
	:
	pivotPoint(pivotPoint), material(material),
	overrideTexDiff(overrideTexDiff),
	overrideTexSpec(overrideTexSpec)
{
	this->meshes.push_back(mesh);
}

Model::~Model()
{
}

void Model::update()
{

}

void Model::render(Shader* shader)
{
	updateUniforms();

	material->sendToShader(shader);

	shader->Use();

	overrideTexDiff->bindTexture(0);
	overrideTexSpec->bindTexture(1);

	for (auto& mesh : meshes) {
		mesh->render(shader);
	}

}

void Model::rotate(glm::vec3 rotation)
{
	for (auto& mesh : meshes) {
		mesh->rotate(rotation);
	}
}

void Model::updateUniforms()
{

}
