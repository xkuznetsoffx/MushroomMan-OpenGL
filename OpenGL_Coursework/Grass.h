#pragma once  

#include <vector>  
#include <glm/glm.hpp>  
#include <GL/glew.h>  
#include "Shader.h"  
#include "Terrain.h"  

struct GrassInstance {
    glm::vec3 position;
    float scale;
};

class Grass {
private:
    std::vector<GrassInstance> grassInstances;
    GLuint VAO, VBO;
    std::unique_ptr<Texture> grassTexture;
    std::unique_ptr<Texture> redFlowerTexture;
    std::unique_ptr<Texture> yellowFlowerTexture;


    Terrain* terrain;

    void setupVAO();

public:
    Grass(Terrain* terrain, int grassCount, const std::string& path = "");
    ~Grass();

    void render(Shader* shader);
    void loadTexture();
};
