#pragma once
#include "libs.h"

enum ShaderType
{
	SHADER_OBJ = 0,
	SHADER_LAMP = 1
};

enum TextureType
{
	TEX_CONTAINER_DIFMAP = 0,
	TEX_CONTAINER_SPECMAP
};

enum MeshEnum
{
	MESH_QUAD = 0,
	MESH_BOX,
	MESH_LAMP
};

class Game
{
public:
	Game(
		const char* title,
		const int width, const int height,
		const int GLmajorVersion,
		const int GLminorVersion,
		bool resizable
	);
	~Game();

	int getWindowShouldClose();

	void setWindowShouldCloes();

	void update();
	void render();

private:
	//variables
	//Window options
	GLFWwindow* window;
	const int WINDOW_WIDTH;
	const int WINDOW_HEIGHT;
	int framebufferWidth;
	int framebufferHeight;

	//OpenGL options
	const int GL_VERSION_MAJOR;
	const int GL_VERSION_MINOR;

	//Camera and matrices
	Camera camera;
	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;
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

	//Shaders
	std::vector<std::unique_ptr<Shader>> shaders;

	//Textures
	std::vector<std::unique_ptr<Texture>> textures;

	//Materials
	std::vector<std::unique_ptr<Material>> materials;

	//Meshes
	std::vector<std::unique_ptr<Mesh>> meshesObjects;
	std::vector<std::unique_ptr<Mesh>> meshesLamps;

	//Lights
	std::unique_ptr<DirectionLight> directionLight;
	std::vector<std::unique_ptr<PointLight>> pointLights;
	std::unique_ptr<SpotLight> spotLight;

	//functions
	void initGLFW();
	void initWindow(const char* title, bool resizable);
	void initGLEW();
	void initOpenGLOptions();
	void initMatrices();
	void initShaders();
	void initTextures();
	void initMaterials();
	void initMeshes();
	void initLights();
	void initUniforms();

	void updateUniforms();

	void updateInput(int key, int action);
	void updateMouse(double xpos, double ypos);
	void updateDeltaTime();

	void do_movment();
	//static functions
public:
	static void framebuffer_resize_callback(GLFWwindow* window, int fbW, int fbH);
	static void  key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
};

