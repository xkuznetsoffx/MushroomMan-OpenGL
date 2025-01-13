#include "Game.h"

#define DEBUG

Game::Game(
	const char* title,
	const int width, const int height,
	const int GLmajorVersion, const int GLminorVersion,
	bool resizable, 
	bool fullscreen
)
	:
	WINDOW_WIDTH(width), WINDOW_HEIGHT(height),
	GL_VERSION_MAJOR(GLmajorVersion), GL_VERSION_MINOR(GLminorVersion),
	camera(glm::vec3(25.f, -0.16f, 25.f))
{
	window = NULL;

	framebufferWidth = width;
	framebufferHeight = height;

	nearPlane = 0.1f;
	farPlane = 1000.0f;

	lastX = static_cast<GLfloat>(WINDOW_WIDTH / 2);
	lastY = static_cast<GLfloat>(WINDOW_HEIGHT / 2);

	//init functions
	initGLFW();
	initWindow(title, resizable, fullscreen);
	initGLEW();
	initOpenGLOptions();
	initMatrices();
	initShaders();
	initTextures();
	initMaterials();
	initSkybox();
	initText();
	initSounds();
	initMeshes();
	initTerrain();
	initModels();
	initLights();
	initUniforms();
	initCallbacks();
	
	healthbar = std::make_unique<HealthBar>(
		100.f,
		glm::vec2(20.f, 20.f),
		glm::vec2(200.f, 20.f)
	);

	grass = std::make_unique<Grass>(
		terrain.get(),
		1000
	);
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

void Game::gameOver()
{
	isGameOver = true;
	camera.SetPosition(glm::vec3(100.f, 0.f, 100.f));
}

void Game::restartGame()
{
	isGameOver = false;
	scores = 0;
	camera.SetPosition(glm::vec3(25.f, -0.16f, 25.f));
	healthbar->setHealth(100.f);
}

void Game::setWindowShouldClose()
{
	glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void Game::update()
{
	glfwPollEvents();
	updateDeltaTime();
	do_movment();

	updateModels();

	camera.update();

	healthbar->update(deltaTime);

	if (!healthbar->isAlive() && !isGameOver)
		gameOver();
	if(isGameOver)
		camera.ProcessMouseMovement(0.25f, 0.f);
}

void Game::render()
{

	glClearColor(0.53f, 0.81f, 0.92f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	updateUniforms();

	healthbar->render(shaders[SHADER_HEALTH].get());

	terrain->render(shaders[SHADER_OBJ].get());

	for (const auto& burger : burgers) {
		burger->render(shaders[SHADER_OBJ].get());
	}

	for (const auto& cola : drinks) {
		cola->render(shaders[SHADER_OBJ].get());
	}

	for (const auto& lamp : meshesLamps){
		lamp->render(shaders[SHADER_LAMP].get());
	}

	skybox->render(shaders[SHADER_SKYBOX].get());
	

	grass->render(shaders[SHADER_GRASS].get());

	if (!isGameOver)
		textManager->render(shaders[SHADER_TEXT].get(),
			"Score: " + std::to_string(scores), 10.0f, 565.f, 0.35f, glm::vec3(0.9f, 0.84f, 0.564f)
		);
	else
	{
		textManager->render(shaders[SHADER_TEXT].get(),
			"GAME OVER!", WINDOW_WIDTH/4.f, WINDOW_HEIGHT / 2 , 0.8f, glm::vec3(0.9f, 0.1f, 0.1f)
		);
		textManager->render(shaders[SHADER_TEXT].get(),
			("You have scored " + std::to_string(scores) + " points").c_str(),
			WINDOW_WIDTH/40.f, WINDOW_HEIGHT / 2 - 100.f, 0.8f, glm::vec3(0.9f, 0.1f, 0.1f)
		);
		textManager->render(shaders[SHADER_TEXT].get(),
			"Press 'ESC' to exit or 'R' to restart the game",
			WINDOW_WIDTH * 0.01f, WINDOW_HEIGHT * 0.01f, 0.3f, glm::vec3(0.9f, 0.84f, 0.564f)
		);
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

void Game::initWindow(const char* title, bool resizable, bool fullscreen)
{
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GL_VERSION_MAJOR);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GL_VERSION_MINOR);
	glfwWindowHint(GLFW_RESIZABLE, resizable);
	glfwWindowHint(GLFW_SAMPLES, 4);

	if (fullscreen) {
		const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		window = glfwCreateWindow(mode->width, mode->height, title, glfwGetPrimaryMonitor(), NULL);
	}
	else {
		window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, title, NULL, NULL);
	}

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
	glEnable(GL_MULTISAMPLE);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

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
	orthoProjection = glm::ortho(0.0f, static_cast<float>(WINDOW_WIDTH), 0.0f, static_cast<float>(WINDOW_HEIGHT));
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

	shaders.push_back(
		std::make_unique<Shader>
		(GL_VERSION_MAJOR, GL_VERSION_MINOR,"HealthBar.vs","HealthBar.frag")
	);

	shaders.push_back(
		std::make_unique<Shader>
		(GL_VERSION_MAJOR, GL_VERSION_MINOR, "TextShader.vs", "TextShader.frag")
	);

	shaders.push_back(
		std::make_unique<Shader>
		(GL_VERSION_MAJOR, GL_VERSION_MINOR, "Skybox.vs", "Skybox.frag")
	);

	shaders.push_back(
		std::make_unique<Shader>
		(GL_VERSION_MAJOR, GL_VERSION_MINOR, "grass.vs", "grass.frag")
	);
}

void Game::initTextures()
{
	textures.push_back(
		std::make_shared<Texture>
		("assets/textures/container2.png", GL_TEXTURE_2D, aiTextureType_DIFFUSE)
	);

	textures.push_back(
		std::make_shared<Texture>
		("assets/textures/container2_specular.png", GL_TEXTURE_2D, aiTextureType_SPECULAR)
	);

	textures.push_back(
		std::make_shared<Texture>
		("assets/textures/brick_wall_diff.png", GL_TEXTURE_2D, aiTextureType_DIFFUSE)
	);

	textures.push_back(
		std::make_shared<Texture>
		("assets/textures/brick_wall_spec.png", GL_TEXTURE_2D, aiTextureType_SPECULAR)
	);
	textures.push_back(
		std::make_shared<Texture>
		("assets/textures/grass-7_diff.jpg", GL_TEXTURE_2D, aiTextureType_DIFFUSE)
	);
	textures.push_back(
		std::make_shared<Texture>
		("assets/textures/grass-7_spec.jpg", GL_TEXTURE_2D, aiTextureType_SPECULAR)
	);
}

void Game::initMaterials()
{
	materials.push_back(
		std::make_unique<Material>(
			textures[TEX_CONTAINER_DIFMAP].get(),
			textures[TEX_CONTAINER_SPECMAP].get(),
			32.f
		)
	);

	materials.push_back(
		std::make_unique<Material>(
			textures[TEX_WALL_DIFMAP].get(),
			textures[TEX_WALL_SPECMAP].get(),
			8.f
		)
	);
	materials.push_back(
		std::make_unique<Material>(
			textures[TEX_GRASS_DIFF].get(),
			textures[TEX_GRASS_SPEC].get(),
			8.f
		)
	);
}

void Game::initMeshes()
{
	//Lamp
	meshesLamps.push_back(
		std::make_unique<Mesh>(
			Cube(),							//primitive
			glm::vec3(3.0f, 4.0f, 3.0f),	//position
			glm::vec3(0.0f),				//rotation
			glm::vec3(0.25f)				//scale
		)
	);
}

void Game::initModels()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> disX(0, terrain->getHeight() - 2);
	std::uniform_int_distribution<> disZ(0, terrain->getWidth() - 2);

	Model burger(
		"assets\\models\\burger\\scene.gltf",
		glm::vec3(0.0f, 0.f, 0.0f)
	);
	burger.scaleUp(glm::vec3(-0.5f));

	for (size_t i = 0; i < 8; ++i) {
		burgers.emplace_back(
			new Model(burger)
		);
		int x = disX(gen);
		int z = disZ(gen);
		burgers[i]->setPosition(glm::vec3(x, terrain->getCurrentHeightFromMap(x, z) + 0.7f, z));
	}

	Model drink(
		"assets\\models\\lit_energy\\scene.gltf",
		glm::vec3(0.0f, 0.f, 0.0f)
	);
	drink.scaleUp(glm::vec3(-0.8f));

	for (size_t i = 0; i < 2; ++i) {
		drinks.emplace_back(
			new Model(drink)
		);
		int x = disX(gen);
		int z = disZ(gen);
		drinks[i]->setPosition(glm::vec3(x, terrain->getCurrentHeightFromMap(x, z) + 0.7f, z));
	}

}

void Game::initLights()
{
	glm::vec3 source = glm::vec3(-10.0f, 15.0f, -10.0f);
	glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 lightDirection = glm::normalize(target - source);

	directionLight = std::make_unique<DirectionLight>(
		glm::vec3(0.2f, 0.2f, 0.2f),		// Ambient 
		glm::vec3(0.8f, 0.7f, 0.6f),		// Diffuse 
		glm::vec3(0.4f, 0.35f, 0.3f),		// Specular 
		lightDirection						// Direction 
	);

	pointLights.push_back(
		std::make_unique<PointLight>(
			glm::vec3(0.05f, 0.05f, 0.05f), //ambient
			glm::vec3(0.8f, 0.8f, 0.8f),	//diffuse
			glm::vec3(1.0f, 1.0f, 1.0f),	//specular
			meshesLamps[0]->getPosition(),	//position
			1.0f,							//constant
			0.18f,							//linear
			0.064f							//quadratic
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
	shaders[SHADER_OBJ]->setMat4("view", viewMatrix);
	shaders[SHADER_OBJ]->setMat4("projection", projectionMatrix);

	shaders[SHADER_LAMP]->setMat4("view", viewMatrix);
	shaders[SHADER_LAMP]->setMat4("projection", projectionMatrix);
	shaders[SHADER_LAMP]->setVec3("lightColor", glm::vec3(1.f));

	shaders[SHADER_HEALTH]->setMat4("projection", orthoProjection);

	shaders[SHADER_TEXT]->setMat4("projection", orthoProjection);

	shaders[SHADER_SKYBOX]->setMat4("view", viewMatrix);
	shaders[SHADER_SKYBOX]->setMat4("projection", projectionMatrix);

	shaders[SHADER_GRASS]->setMat4("view", viewMatrix);
	shaders[SHADER_GRASS]->setMat4("projection", projectionMatrix);

}

void Game::initCallbacks()
{
	glfwSetWindowUserPointer(window, this);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
}

void Game::initTerrain()
{
	terrain = std::make_shared<Terrain>(50, 50, 0.05f, materials[2].get());
}

void Game::initSkybox()
{
	std::vector<std::string> faces
	{
		"assets\\skybox\\right.jpg",
		"assets\\skybox\\left.jpg",
		"assets\\skybox\\top.jpg",
		"assets\\skybox\\bottom.jpg",
		"assets\\skybox\\front.jpg",
		"assets\\skybox\\back.jpg"
	};

	skybox = std::make_unique<Skybox>(faces);
}

void Game::initText()
{
	textManager = std::make_unique<Text>("assets\\fonts\\DkHandRegular-orna.ttf", 128);
}

void Game::initSounds()
{
	sounds.emplace_back(std::make_unique<Sound>("assets\\sounds\\eat1.wav"));
	sounds.back()->setVolume(0.4f);
	sounds.emplace_back(std::make_unique<Sound>("assets\\sounds\\drink.wav"));
	sounds.back()->setVolume(0.4f);
	sounds.emplace_back(std::make_unique<Sound>("assets\\sounds\\spotlight_on.wav"));
	sounds.back()->setVolume(0.1f);
	sounds.emplace_back(std::make_unique<Sound>("assets\\sounds\\spotlight_off.wav"));
	sounds.back()->setVolume(0.1f);
}

void Game::updateUniforms()
{
	shaders[SHADER_OBJ]->Use();

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

	if(framebufferHeight)
		projectionMatrix = glm::perspective(
			glm::radians(camera.GetZoom()),
			static_cast<GLfloat>(framebufferWidth) / static_cast<GLfloat>(framebufferHeight),
			nearPlane, farPlane
		);

	shaders[SHADER_OBJ]->setMat4("view", viewMatrix);
	shaders[SHADER_OBJ]->setMat4("projection", projectionMatrix);

	shaders[SHADER_LAMP]->Use();

	shaders[SHADER_LAMP]->setMat4("view", viewMatrix);
	shaders[SHADER_LAMP]->setMat4("projection", projectionMatrix);

	shaders[SHADER_LAMP]->setVec3("lightColor", glm::vec3(1.f));

	orthoProjection = glm::ortho(0.0f, static_cast<float>(WINDOW_WIDTH), 0.0f, static_cast<float>(WINDOW_HEIGHT));
	shaders[SHADER_HEALTH]->Use();
	shaders[SHADER_HEALTH]->setMat4("projection", orthoProjection);
	shaders[SHADER_TEXT]->Use();
	shaders[SHADER_TEXT]->setMat4("projection", orthoProjection);

	shaders[SHADER_SKYBOX]->Use();

	shaders[SHADER_SKYBOX]->setMat4("view", viewMatrix);
	shaders[SHADER_SKYBOX]->setMat4("projection", projectionMatrix);

	shaders[SHADER_GRASS]->Use();

	shaders[SHADER_GRASS]->setMat4("view", viewMatrix);
	shaders[SHADER_GRASS]->setMat4("projection", projectionMatrix);

	shaders[SHADER_GRASS]->Unuse();

}

void Game::updateModels()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> disX(0, terrain->getHeight() - 2);
	std::uniform_int_distribution<> disZ(0, terrain->getWidth() - 2);

	for (const auto& obj : burgers) {
		obj->move(glm::vec3(0.0f, sin(glfwGetTime()) * deltaTime * 0.1f, 0.0f));
		obj->rotate(glm::vec3(0.0f, 45.f * deltaTime, 0.0f)); ;
		if (checkCollision(obj->getHitbox(), camera.getHitbox())) {
			int x = disX(gen);
			int z = disZ(gen);
			obj->setPosition(glm::vec3(x, terrain->getCurrentHeightFromMap(x, z) + 0.7f, z));
			healthbar->increaseHealth(10.0f);
			sounds[SOUND_EAT]->play();
			scores++;
		}
	}

	for (const auto& obj : drinks) {
		obj->move(glm::vec3(0.0f, sin(glfwGetTime()) * deltaTime * 0.1f, 0.0f));
		obj->rotate(glm::vec3(0.0f, 45.f * deltaTime, 0.0f)); ;
		if (checkCollision(obj->getHitbox(), camera.getHitbox())) {
			int x = disX(gen);
			int z = disZ(gen);
			obj->setPosition(glm::vec3(x, terrain->getCurrentHeightFromMap(x, z) + 0.7f, z));
			healthbar->increaseHealth(5.0f);
			camera.updateCameraSpeed(2.0f, 3.0f);
			sounds[SOUND_DRINK]->play();
		}
	}
}

void Game::updateInput(int key, int action)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		setWindowShouldClose();
	if (!isGameOver) {
		if (key == GLFW_KEY_F && action == GLFW_PRESS) {
			if (spotLight->isOn())
			{
				spotLight->turnOff();
				sounds[SOUND_SPOTLIGHT_OFF]->play();
			}
			else
			{
				spotLight->turnOn();
				sounds[SOUND_SPOTLIGHT_ON]->play();
			}
		}

#ifdef DEBUG
		if (key == GLFW_KEY_TAB && action == GLFW_PRESS)
			std::cout << "Camera position: " <<
			camera.GetPoistion().x << ' ' <<
			camera.GetPoistion().y << ' ' <<
			camera.GetPoistion().z << '\n';
#endif // DEBUG

		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
	else if ((key == GLFW_KEY_R && action == GLFW_PRESS)) {
			restartGame();
	}
	
}
	

void Game::updateMouse(double xpos, double ypos)
{
	if (!isGameOver) {
		if (firstMouse) {
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
	
}

void Game::updateDeltaTime()
{
	currentFrame = static_cast<GLfloat>(glfwGetTime());
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
}

void Game::do_movment()
{
	std::vector<Camera_Movement> directions;

	if (keys[GLFW_KEY_W]){
		directions.push_back(FORWARD);
	}
	if (keys[GLFW_KEY_S]) {
		directions.push_back(BACKWARD);
	}
	if (keys[GLFW_KEY_A]) {
		directions.push_back(LEFT);
	}
	if (keys[GLFW_KEY_D]) {
		directions.push_back(RIGHT);
	}

	camera.ProcessKeyboard(directions, deltaTime, *terrain.get());
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

void Game::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	Game* game = static_cast<Game*>(glfwGetWindowUserPointer(window));
	if (game) {
		game->camera.ProcessMouseScroll(yoffset);
	}
}
	

