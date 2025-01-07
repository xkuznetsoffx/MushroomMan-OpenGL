#pragma once

#include <memory>
#include <thread>
#include <chrono>
#include "Mesh.h"
#include "Material.h"
#include "Collision.h"

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

	Model(
		const Model& oth
	);

	~Model();
	void update();

	void render(Shader* shader);

	void rotate(const glm::vec3 rotation);
	void move(const glm::vec3 position);
	void scaleUp(const glm::vec3 scale);

	const glm::vec3 getPosition();
	void setPosition(const glm::vec3 position);
	void setYCoord(float y);

	const AABB& getHitbox();
private:
	Material* material;
	std::vector<SPtrMesh> meshes;
	glm::vec3 pivotPoint;
	glm::vec3 origin;

	std::string directory;

	AABB hitbox;

	std::vector<std::shared_ptr<Texture>> textures_loaded;

	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<std::shared_ptr<Texture>> loadMaterialTextures(aiMaterial * mat, aiTextureType type);

	void updateUniforms();
};

