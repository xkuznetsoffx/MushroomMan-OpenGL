#pragma once

#include "libs.h"
#include "HealthBar.h"

enum ShaderEnum
{
	SHADER_OBJ = 0,
	SHADER_LAMP,
	SHADER_HEALTH,
	SHADER_TEXT,
	SHADER_SKYBOX,
	SHADER_GRASS

};

enum TextureEnum
{
	TEX_CONTAINER_DIFMAP = 0,
	TEX_CONTAINER_SPECMAP,
	TEX_WALL_DIFMAP,
	TEX_WALL_SPECMAP,
	TEX_GRASS_DIFF,
	TEX_GRASS_SPEC,
	TEX_GRASS
};

enum MaterialEnum {
	MAT_CONTAINER = 0,
	MAT_WALL,
	MAT_GRASS_TERR,
	MAT_GRASS
};

enum MeshObjectsEnum
{
	MESH_QUAD = 0,
	MESH_BOX
};

enum MeshLampsEnum
{
	MESH_LAMP = 0
};

enum SoundsEnum
{
	SOUND_EAT = 0,
	SOUND_DRINK,
	SOUND_SPOTLIGHT_ON,
	SOUND_SPOTLIGHT_OFF,
	SOUND_SAD
};

class Game
{
public:
	Game(
		const char* title,
		const int width, const int height,
		const int GLmajorVersion,
		const int GLminorVersion,
		bool resizable,
		bool fullscreen = false
	);
	~Game();

	int getWindowShouldClose();

	void update();
	void render();

private:
	//variables
	//Window options
	GLFWwindow* window;
	int WINDOW_WIDTH;
	int WINDOW_HEIGHT;
	int framebufferWidth;
	int framebufferHeight;

	//OpenGL options
	const int GL_VERSION_MAJOR;
	const int GL_VERSION_MINOR;

	//Camera and matrices
	Camera camera;
	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;
	glm::mat4 orthoProjection;
	float nearPlane;
	float farPlane;

	//Frames time
	GLfloat currentFrame = 0.0f;
	GLfloat deltaTime = 0.0f;
	GLfloat lastFrame = 0.0f;

	//Mouse
	GLfloat lastX;
	GLfloat lastY;
	bool firstMouse = true;

	bool keys[1024];

	//Sounds
	std::vector<std::unique_ptr<Sound>> sounds;
	
	//Skybox
	std::unique_ptr<Skybox> skybox;

	//Text
	std::unique_ptr<Text> textManager;
	size_t scores = 0;

	//Shaders
	std::vector<UPtrShader> shaders;

	//Textures
	std::vector<std::shared_ptr<Texture>> textures;

	//Materials
	std::vector<UPtrMaterial> materials;

	//Meshes
	std::vector<UPtrMesh> meshesLamps;

	//Models
	std::vector<SPtrModel> burgers;
	std::vector<SPtrModel> drinks;

	std::shared_ptr<Terrain> terrain;

	//Lights
	UPtrDirLight directionLight;
	std::vector<UPtrPointLight> pointLights;
	UPtrSpotLight spotLight;

	//HP
	std::unique_ptr<HealthBar> healthbar;

	std::unique_ptr<Grass> grass;

	bool isGameOver = false;

	bool sadMusicFlag = true;

	//functions
	void initGLFW();
	void initWindow(const char* title, bool resizable, bool fullscreen);
	void initGLEW();
	void initOpenGLOptions();
	void initMatrices();
	void initShaders();
	void initTextures();
	void initMaterials();
	void initTerrain();
	void initSkybox();
	void initText();
	void initSounds();
	void initMeshes();
	void initModels();
	void initLights();
	void initUniforms();
	void initCallbacks();
	
	void updateUniforms();
	void updateModels();

	void updateInput(int key, int action);
	void updateMouse(double xpos, double ypos);
	void updateDeltaTime();

	void do_movment();

	//static functions
	static void framebuffer_resize_callback(GLFWwindow* window, int fbW, int fbH);
	static void  key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

	void gameOver();
	void gameOverRender();
	void restartGame();

	void setWindowShouldClose();

};

