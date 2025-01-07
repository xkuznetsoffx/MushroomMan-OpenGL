#include "Terrain.h"

Terrain::Terrain(int width, int height, float scale, Material* material)
	:
	width(width), height(height), scale(scale), material(material)
{
	heightMap.resize(width * height);
	generateTerrain();
	initVAO();
}

Terrain::~Terrain()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void Terrain::render(Shader* shader)
{
	shader->Use();

	shader->setMat4("model", glm::mat4(1.f));

	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	material->sendToShader(shader);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, nrOfIndices, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

float Terrain::getCurrentHeightFromMap(float x, float z)
{
    if (!isCoordInMap(x, z)) return 0.f;
   
    float height = 0;
    int intX = static_cast<int>(x);
    int intZ = static_cast<int>(z);

    float h1 = ((1 - (x - intX)) * heightMap[intZ * width + intX] + (x - intX) * heightMap[intZ * width + (intX + 1)]);
    float h2 = ((1 - (x - intX)) * heightMap[(intZ+1) * width + intX] + (x - intX) * heightMap[(intZ+1) * width + (intX + 1)]);


    return (1-(z-intZ))*h1 + (z-intZ)*h2;
}

void Terrain::generateTerrain()
{
	FastNoiseLite noise;
	noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
	noise.SetFrequency(scale);

	float value;

	for(int z = 0; z < height; ++z) {
		for (int x = 0; x < width; ++x) {
			value = noise.GetNoise(static_cast<float>(x), static_cast<float>(z));
			heightMap[z * width + x] = value * 5.f;
		}
	}
}

int Terrain::getHeight()
{
    return height;
}

int Terrain::getWidth()
{
    return width;
}

void Terrain::initVAO()
{
    std::vector<GLuint> indices;
    std::vector<Vertex> vertices;

    /*
     std::vector<GLfloat> normals;
    normals.resize(width * height * 3, 0.0f);
    */
   
    // Заполняем массив вершин  
    for (int z = 0; z < height; ++z) {
        for (int x = 0; x < width; ++x) {
            Vertex vert = {
                glm::vec3(
                    static_cast<GLfloat>(x),
                    heightMap[z * width + x],
                    static_cast<GLfloat>(z)
                ),
                glm::vec3(
                    0.f, 1.f, 0.f 
                ),
                glm::vec2(
                    static_cast<GLfloat>(x) / ((sqrtf(width) / 2)), 
                    static_cast<GLfloat>(z) / ((sqrtf(height) / 2))
                ),
                glm::vec3(0.f)  
            };
            vertices.push_back(vert);
        }
    }

    for (int z = 0; z < height - 1; ++z) {
        for (int x = 0; x < width - 1; ++x) {
            int topLeft = z * width + x;
            int bottomLeft = (z + 1) * width + x;
            int topRight = z * width + (x + 1);
            int bottomRight = (z + 1) * width + (x + 1);

            indices.push_back(topLeft);
            indices.push_back(bottomLeft);
            indices.push_back(topRight);

            indices.push_back(bottomLeft);
            indices.push_back(bottomRight);
            indices.push_back(topRight);
        }
    }

    nrOfIndices = indices.size(); 

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    GLuint EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
  
    //pozition  
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);
    //normal  
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);
    //texcoord 
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texcoord));
    glEnableVertexAttribArray(2);
    //color 
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
    glEnableVertexAttribArray(3);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

bool Terrain::isCoordInMap(float x, float z)
{
    return ( (x >= 0) && (x < width-1) && (z >= 0) && (z < height-1) );
}
