#include "Model.h"

Model::Model(
	glm::vec3 pivotPoint,
	Material* material,
	std::vector<SPtrMesh> meshes
)
	:
	pivotPoint(pivotPoint), material(material)
{
	this->meshes.reserve(meshes.size());
	for (const auto& mesh : meshes) {
		this->meshes.emplace_back
		(
			std::make_shared<Mesh>(*mesh)
		);
		(*(this->meshes.end() - 1))->move(pivotPoint);
	}

		
}

Model::Model(
	glm::vec3 pivotPoint,
	Material* material,
	SPtrMesh mesh
)
	:
	pivotPoint(pivotPoint), material(material)
{
	this->meshes.push_back(mesh);
	(*(this->meshes.end() - 1))->move(pivotPoint);
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
