#pragma once
#include <unordered_map>
#include <string>
enum TextureTypeName
{
	TTN_DIFFUSE = 0,
	TTN_SPECULAR 
};

static std::unordered_map<TextureTypeName, std::string> TTypeNames = {
	{TTN_DIFFUSE, "texture_diffuse"},
	{TTN_SPECULAR, "texture_specular"}
};