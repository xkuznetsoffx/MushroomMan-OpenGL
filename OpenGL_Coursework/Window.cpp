#include "Window.h"

Window::Window(const char* title) {
	glfwInit();

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	int width = glfwGetVideoMode(glfwGetPrimaryMonitor())->width;
	int height = glfwGetVideoMode(glfwGetPrimaryMonitor())->height;
	window.reset(glfwCreateWindow(width, height, title, nullptr, nullptr));

	glfwGetFramebufferSize(window.get(), &framebufferWidth, &framebufferHeight);
	glfwSetFramebufferSizeCallback(window.get(), framebufferResizeCallback);

	if (!window) {
		std::cerr << "ERROR:WINDOW.CPP::CREATE_WINDOW_FAILED" << '\n';
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(window.get());
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) {
		std::cout << "ERROR:WINDOW.CPP::GLEW_INIT_FAILED" << '\n';
		glfwTerminate();
		return;
	}
	
	glViewport(0, 0, width, height);
	glfwSetInputMode(window.get(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	renderer = std::make_unique<Renderer>();

	shader = std::make_unique<Shader>(4,4,"vertex_core.glsl", "fragment_core.glsl");
	glfwSetFramebufferSizeCallback(window.get(), framebufferResizeCallback);
}

Window::~Window() {
	glfwTerminate();
}

GLFWwindow* Window::getWindowPtr() const {
	return window.get();
}

void Window::processEvents() const{
	glfwPollEvents();
}

void Window::framebufferResizeCallback(GLFWwindow* window, int fbW, int fbH) {
	glViewport(0, 0, fbW, fbH);
}

void Window::keyboardInput(void(*key_callback)(GLFWwindow*, int, int, int, int)) const {
	glfwSetKeyCallback(window.get(),key_callback);
}

void Window::mouseInput(void(*mouse_callback)(GLFWwindow*, double, double)) const {
	glfwSetCursorPosCallback(window.get(), mouse_callback);
}

void Window::scrolInput(void(*scroll_callback)(GLFWwindow*, double, double)) const {
	glfwSetScrollCallback(window.get(), scroll_callback);
}
