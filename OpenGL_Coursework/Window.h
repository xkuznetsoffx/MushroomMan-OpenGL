#pragma once
#include "libs.h"
#include "Renderer.h"

using GLFWwindowUniquePtr = std::unique_ptr<GLFWwindow, decltype(&glfwDestroyWindow)>;
using ShaderUniquePtr = std::unique_ptr<Shader>;
using RendererUniquePtr = std::unique_ptr<Renderer>;

class Window {
public:
	Window(const char* title);
	~Window();

	GLFWwindow* getWindowPtr() const;
	void processEvents() const;
	void keyboardInput(void(*key_callback)(GLFWwindow*, int, int, int, int)) const;
	void mouseInput(void(*mouse_callback)(GLFWwindow*, double, double)) const;
	void scrolInput(void(*scroll_callback)(GLFWwindow*, double, double)) const;

private:
	int framebufferWidth;
	int framebufferHeight;
	RendererUniquePtr renderer;
	ShaderUniquePtr shader;
	GLFWwindowUniquePtr window{ nullptr, glfwDestroyWindow };
	static void framebufferResizeCallback(GLFWwindow* window, int fbW, int fbH);
};

