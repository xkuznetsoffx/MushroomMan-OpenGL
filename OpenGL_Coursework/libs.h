#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <chrono>
#include <thread>
#include <cmath>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <SOIL2/SOIL2.h>
#include <SOIL2/stb_image.h>

#include "assimp/types.h"
#include "assimp/material.h"

#include "Camera.h"
#include "Primitives.h"
#include "Light.h"
#include "Model.h"
#include "Terrain.h"

using UPtrShader = std::unique_ptr<Shader>;
using UPtrTexture = std::unique_ptr<Texture>;
using UPtrMaterial = std::unique_ptr<Material>;

using UPtrMesh = std::unique_ptr<Mesh>;
using SPtrMesh = std::shared_ptr<Mesh>;

using UPtrModel = std::unique_ptr<Model>;

using UPtrDirLight = std::unique_ptr<DirectionLight>;
using UPtrPointLight = std::unique_ptr<PointLight>;
using UPtrSpotLight = std::unique_ptr<SpotLight>;