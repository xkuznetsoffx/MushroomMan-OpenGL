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
		("Images/container2.png", GL_TEXTURE_2D, TEX_CONTAINER_DIFMAP)
	);

	textures.push_back(
		std::make_unique<Texture>
		("Images/container2_specular.png", GL_TEXTURE_2D, TEX_CONTAINER_SPECMAP)
	);
}

void Game::initMaterials()
{
	materials.push_back(
		std::make_unique<Material>(
			textures[TEX_CONTAINER_DIFMAP].get()->getUnit(),
			textures[TEX_CONTAINER_SPECMAP].get()->getUnit(),
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
			quad,
			glm::vec3(-3.0f, -1.0f, 0.0f)
		)
	);

	//Box
	meshesObjects.push_back(
		std::make_unique<Mesh>(
			cube
		)
	);

	//Lamp
	meshesLamps.push_back(
		std::make_unique<Mesh>(
			cube,
			glm::vec3(-3.0f, 2.0f, 0.0f)
		)
	);


}

void Game::initLights()
{

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


	//default
	shaders[SHADER_OBJ]->setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
	shaders[SHADER_OBJ]->setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
	shaders[SHADER_OBJ]->setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
	shaders[SHADER_OBJ]->setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
	for (int i = 0; i < meshesLamps.size(); ++i) {
		std::string index = std::to_string(i);
		shaders[SHADER_OBJ]->setVec3("pointLights[" + index + "].position", meshesLamps[0]->getPosition());
		shaders[SHADER_OBJ]->setVec3("pointLights[" + index + "].ambient", 0.05f, 0.05f, 0.05f);
		shaders[SHADER_OBJ]->setVec3("pointLights[" + index + "].diffuse", 0.8f, 0.8f, 0.8f);
		shaders[SHADER_OBJ]->setVec3("pointLights[" + index + "].specular", 1.0f, 1.0f, 1.0f);
		shaders[SHADER_OBJ]->setFloat("pointLights[" + index + "].constant", 1.0f);
		shaders[SHADER_OBJ]->setFloat("pointLights[" + index + "].linear", 0.09f);
		shaders[SHADER_OBJ]->setFloat("pointLights[" + index + "].quadratic", 0.032f);
	}
	shaders[SHADER_OBJ]->setVec3("spotLight.position", camera.GetPoistion());
	shaders[SHADER_OBJ]->setVec3("spotLight.direction", camera.GetFront());
	shaders[SHADER_OBJ]->setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
	shaders[SHADER_OBJ]->setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
	shaders[SHADER_OBJ]->setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
	shaders[SHADER_OBJ]->setFloat("spotLight.constant", 1.0f);
	shaders[SHADER_OBJ]->setFloat("spotLight.linear", 0.09f);
	shaders[SHADER_OBJ]->setFloat("spotLight.quadratic", 0.032f);

	shaders[SHADER_OBJ]->setFloat("spotLight.cutOff", glm::cos(glm::radians(radCutOff)));
	shaders[SHADER_OBJ]->setFloat("spotLight.outerCutOff", glm::cos(glm::radians(radOuterCutOff)));

	viewMatrix = camera.GetViewMatrix();
	glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
	projectionMatrix = glm::perspective(glm::radians(camera.GetZoom()), static_cast<GLfloat>(framebufferWidth) / static_cast<GLfloat>(framebufferHeight), 0.1f, 100.0f);

	shaders[SHADER_OBJ]->setMat4("view", viewMatrix);
	shaders[SHADER_OBJ]->setMat4("projection", projectionMatrix);

	textures[TEX_CONTAINER_DIFMAP]->bindTexture();
	textures[TEX_CONTAINER_SPECMAP]->bindTexture();

	shaders[SHADER_LAMP]->Use();

	shaders[SHADER_LAMP]->setMat4("view", viewMatrix);
	shaders[SHADER_LAMP]->setMat4("projection", projectionMatrix);

	shaders[SHADER_LAMP]->setVec3("lightColor", glm::vec3(1.f));
}

void Game::framebuffer_resize_callback(GLFWwindow* window, int fbW, int fbH)
{
	glViewport(0, 0, fbW, fbH);
}