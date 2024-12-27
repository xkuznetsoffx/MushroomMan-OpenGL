#pragma once

#include <memory>
#include "Mesh.h"
#include "Material.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
	
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
	
	Model(
		std::string path,
		glm::vec3 pivotPoint,
		glm::vec3 origin = glm::vec3(0.f)
	);

	~Model();
	void update();

	void render(Shader* shader);

	void rotate(const glm::vec3 rotation);
	void move(const glm::vec3 position);
	void scaleUp(const glm::vec3 scale);

private:
	Material* material;
	std::vector<SPtrMesh> meshes;
	glm::vec3 pivotPoint;
	glm::vec3 origin;

	std::string directory;

	std::vector<std::shared_ptr<Texture>> textures_loaded;

	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<std::shared_ptr<Texture>> loadMaterialTextures(aiMaterial * mat, aiTextureType type);

	void updateUniforms();
};

