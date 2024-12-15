#pragma once
#include "libs.h"


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
	GLFWwindow* window;

	const int WINDOW_WIDTH;
	const int WINDOW_HEIGHT;
	int framebufferWidth;
	int framebufferHeight;

	const int GL_VERSION_MAJOR;
	const int GL_VERSION_MINOR;

	Camera camera;

	glm::mat4 viewMatrix;

	glm::mat4 projectionMatrix;
	float nearPlane;
	float farPlane;

	//functions
	void initGLFW();
	void initWindow(const char* title, bool resizable);
	void initGLEW();
	void initOpenGLOptions();
	void initMatrices();

	//static functions
public:
	static void framebuffer_resize_callback(GLFWwindow* window, int fbW, int fbH);
};

