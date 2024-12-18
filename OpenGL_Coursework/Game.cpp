#include "Game.h"

Game::Game(const char* title,
	const int width, const int height,
	const int GLmajorVersion, const int GLminorVersion,
	bool resizable) :
	WINDOW_WIDTH(width), WINDOW_HEIGHT(height),
	GL_VERSION_MAJOR(GLmajorVersion), GL_VERSION_MINOR(GLminorVersion),
	camera(glm::vec3(-2.0f, 0.0f, 6.0f))
{
	window = NULL;

	framebufferWidth = width;
	framebufferHeight = height;

	nearPlane = 0.1f;
	farPlane = 100.0f;

	lastX = static_cast<GLfloat>(WINDOW_WIDTH / 2);
	lastY = static_cast<GLfloat>(WINDOW_HEIGHT / 2);

	//init functions
	initGLFW();
	initWindow(title, resizable);
	initGLEW();
	initOpenGLOptions();
	initMatrices();
	initShaders();
	initTextures();
	initMaterials();
	initMeshes();
	initLights();
	initUniforms();


	//callback functions
	glfwSetWindowUserPointer(window, this);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
}

Game::~Game()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

int Game::getWindowShouldClose()
{
	return glfwWindowShouldClose(window);
}

void Game::setWindowShouldCloes()
{
	glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void Game::update()
{
	glfwPollEvents();
	updateDeltaTime();
	do_movment();
}

void Game::render()
{
	glClearColor(0.f, 0.f, 0.f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	updateUniforms();

	shaders[SHADER_OBJ]->Use();
	for (auto& meshObj : meshesObjects) {
		meshObj->render(shaders[SHADER_OBJ].get());
	}
	

	shaders[SHADER_LAMP]->Use();
	for (auto& meshLamp : meshesLamps) {
		meshLamp->render(shaders[SHADER_LAMP].get());
	}

	glfwSwapBuffers(window);
	glFlush();

	glBindVertexArray(0);
	glUseProgram(0);
	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Game::initGLFW()
{
	if (glfwInit() == GLFW_FALSE) {
		std::cerr << "ERROR::GAME::INIT_GLFW" << '\n';
		glfwTerminate();
		return;
	}
}

void Game::initWindow(const char* title, bool resizable)
{
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GL_VERSION_MAJOR);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GL_VERSION_MINOR);
	glfwWindowHint(GLFW_RESIZABLE, resizable);

	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, title, NULL, NULL);

	if (window == NULL) {
		std::cerr << "ERROR::GAME::INIT_WINDOW" << '\n';
		glfwTerminate();
	}

	glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
	glfwSetFramebufferSizeCallback(window, Game::framebuffer_resize_callback);

	glfwMakeContextCurrent(window);
}

void Game::initGLEW()
{
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) {
		std::cerr << "ERROR::GAME::INIT_GLEW" << '\n';
		glfwTerminate();
	}
}

void Game::initOpenGLOptions()
{
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Game::initMatrices()
{
	viewMatrix = camera.GetViewMatrix();

	projectionMatrix = glm::mat4(1.f);
	projectionMatrix = glm::perspective(
		glm::radians(camera.GetZoom()),
		static_cast<GLfloat>(framebufferWidth) / static_cast<GLfloat>(framebufferHeight),
		nearPlane, farPlane
	);
}

void Game::initShaders()
{
	shaders.push_back(
		std::make_unique<Shader>
		(GL_VERSION_MAJOR, GL_VERSION_MINOR, "objects.vs", "objects.frag")
	);

	shaders.push_back(
		std::make_unique<Shader>
		(GL_VERSION_MAJOR, GL_VERSION_MINOR, "lamp.vs", "lamp.frag")
	);
}

void Game::initTextures()
{
	textures.push_back(
		std::make_unique<Texture>
		("Images/container2.png", GL_TEXTURE_2D)
	);

	textures.push_back(
		std::make_unique<Texture>
		("Images/container2_specular.png", GL_TEXTURE_2D)
	);
}

void Game::initMaterials()
{
	materials.push_back(
		std::make_unique<Material>(
			TEX_CONTAINER_DIFMAP,
			TEX_CONTAINER_SPECMAP,
			32.f
		)
	);
}

void Game::initMeshes()
{
	Quad quad;
	Cube cube;
	//Quad
	meshesObjects.push_back(
		std::make_unique<Mesh>(
			quad,							//primitive
			glm::vec3(-3.0f, -1.0f, 0.0f)	//position
		)
	);

	//Box
	meshesObjects.push_back(
		std::make_unique<Mesh>(
			cube							//primitive
		)
	);

	//Lamp
	meshesLamps.push_back(
		std::make_unique<Mesh>(
			cube,							//primitive
			glm::vec3(-3.0f, 2.0f, 0.0f)	//position
		)
	);


}

void Game::initLights()
{
	directionLight = std::make_unique<DirectionLight>(
		glm::vec3(0.05f, 0.05f, 0.05f),		//ambient
		glm::vec3(0.4f, 0.4f, 0.4f),		//diffuse
		glm::vec3(0.5f, 0.5f, 0.5f),		//specular
		glm::vec3(-0.2f, -1.0f, -0.3f)		//direction
	);

	pointLights.push_back(
		std::make_unique<PointLight>(
			glm::vec3(0.05f, 0.05f, 0.05f), //ambient
			glm::vec3(0.8f, 0.8f, 0.8f),	//diffuse
			glm::vec3(1.0f, 1.0f, 1.0f),	//specular
			meshesLamps[0]->getPosition(),	//position
			1.0f,							//constant
			0.09f,							//linear
			0.032f							//quadratic
		)
	);

	spotLight = std::make_unique<SpotLight>(
		glm::vec3(0.0f, 0.0f, 0.0f),		//ambient			
		glm::vec3(1.0f, 1.0f, 1.0f),		//diffuse
		glm::vec3(1.0f, 1.0f, 1.0f),		//specular
		camera.GetPoistion(),				//position
		camera.GetFront(),					//direction
		1.0f,								//constant
		0.09f,								//linear
		0.032f,								//quadratic
		10.0f,								//radCutOff
		15.0f								//radOuterCutOff
	);
}

void Game::initUniforms()
{
	shaders[SHADER_OBJ].get()->setMat4("view", viewMatrix);
	shaders[SHADER_OBJ].get()->setMat4("projection", projectionMatrix);

	shaders[SHADER_LAMP].get()->setMat4("view", viewMatrix);
	shaders[SHADER_LAMP].get()->setMat4("projection", projectionMatrix);
	shaders[SHADER_LAMP].get()->setVec3("lightColor", glm::vec3(1.f));

}

void Game::updateUniforms()
{
	shaders[SHADER_OBJ]->Use();
	materials[0]->sendToShader(shaders[SHADER_OBJ].get());

	shaders[SHADER_OBJ]->setVec3("viewPos", camera.GetPoistion());

	directionLight->sendToShader(shaders[SHADER_OBJ].get());

	for (int i = 0; i < pointLights.size(); ++i) {
		pointLights[i]->sendToShader(shaders[SHADER_OBJ].get());
	}

	spotLight->updatePosition(camera.GetPoistion());
	spotLight->updateDirection(camera.GetFront());
	spotLight->sendToShader(shaders[SHADER_OBJ].get());

	viewMatrix = camera.GetViewMatrix();
	glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
	projectionMatrix = glm::perspective(glm::radians(camera.GetZoom()), static_cast<GLfloat>(framebufferWidth) / static_cast<GLfloat>(framebufferHeight), 0.1f, 100.0f);

	shaders[SHADER_OBJ]->setMat4("view", viewMatrix);
	shaders[SHADER_OBJ]->setMat4("projection", projectionMatrix);

	textures[TEX_CONTAINER_DIFMAP]->bindTexture(TEX_CONTAINER_DIFMAP);
	textures[TEX_CONTAINER_SPECMAP]->bindTexture(TEX_CONTAINER_SPECMAP);

	shaders[SHADER_LAMP]->Use();

	shaders[SHADER_LAMP]->setMat4("view", viewMatrix);
	shaders[SHADER_LAMP]->setMat4("projection", projectionMatrix);

	shaders[SHADER_LAMP]->setVec3("lightColor", glm::vec3(1.f));
}

void Game::updateInput(int key, int action)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		setWindowShouldCloes();
	if (key == GLFW_KEY_F && action == GLFW_PRESS) {
		if (spotLight->isOn())
			spotLight->turnOff();
		else
			spotLight->turnOn();
	}
	if (action == GLFW_PRESS)
		keys[key] = true;
	else if (action == GLFW_RELEASE)
		keys[key] = false;
}

void Game::updateMouse(double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void Game::updateDeltaTime()
{
		currentFrame = static_cast<GLfloat>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
}

void Game::do_movment()
{

	if (keys[GLFW_KEY_W])
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (keys[GLFW_KEY_S])
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A])
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (keys[GLFW_KEY_D])
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (keys[GLFW_KEY_SPACE])
		camera.ProcessKeyboard(UP, deltaTime);
	if (keys[GLFW_KEY_LEFT_SHIFT])
		camera.ProcessKeyboard(DOWN, deltaTime);
	
}

void Game::framebuffer_resize_callback(GLFWwindow* window, int fbW, int fbH)
{
	glViewport(0, 0, fbW, fbH);
}

void Game::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Game* game = static_cast<Game*>(glfwGetWindowUserPointer(window));
	if (game) {
		game->updateInput(key, action);
	}
}

void Game::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	Game* game = static_cast<Game*>(glfwGetWindowUserPointer(window));
	if (game) {
		game->updateMouse(xpos, ypos);
	}
}
	

