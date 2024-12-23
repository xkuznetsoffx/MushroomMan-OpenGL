#include "Material.h"

Material::Material(Texture* diffuse, Texture* specular, float shininess) :
	diffuse(diffuse),
	specular(specular),
	shininess(shininess) 
{

}

Material::~Material()
{
}

void Material::sendToShader(Shader* program)
{
	diffuse->bindTexture(0);
	specular->bindTexture(1);

	program->setInt("material.diffuse", 0);
	program->setInt("material.specular", 1);
	program->setFloat("material.shininess", shininess);

}


