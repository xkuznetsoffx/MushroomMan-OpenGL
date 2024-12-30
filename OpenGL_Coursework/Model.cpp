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

Model::Model(std::string path, glm::vec3 pivotPoint, glm::vec3 origin) 
	:
	pivotPoint(pivotPoint), origin(origin)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_PreTransformVertices | aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices  );
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}

	directory = path.substr(0, path.find_last_of('/'));
	if (directory == path)
		directory = path.substr(0, path.find_last_of('\\'));

	processNode(scene->mRootNode, scene);
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

	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	if(material)
		material->sendToShader(shader);

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

void Model::move(const glm::vec3 position)
{
	pivotPoint += position;
	for (auto& mesh : meshes) {
		mesh->setPosition(pivotPoint);
	}
}

void Model::scaleUp(const glm::vec3 scale)
{
	for (auto& mesh : meshes) {
		mesh->scaleUp(scale);
	}
}

const glm::vec3 Model::getPosition()
{
	return pivotPoint;
}

void Model::setYCoord(float y)
{
	pivotPoint = glm::vec3(
		pivotPoint.x,
		y,
		pivotPoint.z
	);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
	for (size_t i = 0; i < node->mNumMeshes; ++i) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		Mesh newMesh = processMesh(mesh, scene);
		newMesh.move(pivotPoint);
		newMesh.setOrigin(origin);
		meshes.emplace_back(std::make_shared<Mesh>(newMesh));
	}

	for (size_t i = 0; i < node->mNumChildren; ++i)
		processNode(node->mChildren[i], scene);
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<std::shared_ptr<Texture>> textures;

	vertices.reserve(mesh->mNumVertices);
	indices.reserve(mesh->mNumFaces * 3);

	for (size_t i = 0; i < mesh->mNumVertices; ++i) {
		Vertex vertex;

		const aiVector3D& vertexData = mesh->mVertices[i];
		const aiVector3D& normalData = mesh->mNormals[i];

		vertex.position = glm::vec3(vertexData.x, vertexData.y, vertexData.z);
		vertex.normal = glm::vec3(normalData.x, normalData.y, normalData.z);

		if (mesh->mTextureCoords[0]) {
			const auto& texCoordData = mesh->mTextureCoords[0][i];
			vertex.texcoord = glm::vec2(texCoordData.x, texCoordData.y);
		}
		else {
			vertex.texcoord = glm::vec2(0.0f, 0.0f);
		}

		vertices.push_back(vertex);
	}

	for (size_t i = 0; i < mesh->mNumFaces; ++i) {
		aiFace face = mesh->mFaces[i];
		indices.insert(indices.end(), face.mIndices, face.mIndices + face.mNumIndices);
	}
	
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

	std::vector<std::shared_ptr<Texture>> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE);
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	// 2. specular maps
	std::vector<std::shared_ptr<Texture>> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR);
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

	return Mesh(vertices.data(),mesh->mNumVertices,
		indices.data(), indices.size(), textures);
}

std::vector<std::shared_ptr<Texture>> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type)
{
	std::vector<std::shared_ptr<Texture>> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		bool skip = false;
		std::string path = directory + '\\' + str.C_Str();
		for (unsigned int j = 0; j < textures_loaded.size(); j++)
		{
			if (textures_loaded[j]->getPath() == path)
			{
				textures.push_back(textures_loaded[j]);
				skip = true;
				break;
			}
		}
		if (!skip)
		{
			auto texture = std::make_shared<Texture>(path.c_str(), GL_TEXTURE_2D, type);
			textures.push_back(texture);
			textures_loaded.push_back(texture);
		}	
	}
	return textures;
}


void Model::updateUniforms()
{

}
