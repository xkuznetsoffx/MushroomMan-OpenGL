#include "Material.h"

Material::Material(GLint diffuse, GLint specular, float shininess) :
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
	program->setInt("material.diffuse", diffuse);
	program->setInt("material.specular", specular);
	program->setFloat("material.shininess", shininess);

}


