#include "Game.h"
#include "Window.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

void do_movement();

int screenWidth = 1920;
int screenHeight = 1080;
int framebufferWidth = 1920;
int framebufferHeight = 1080;
Camera camera(glm::vec3(-2.0f, 0.0f, 6.0f));


bool keys[1024];

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

GLfloat lastX = static_cast<GLfloat>(screenWidth / 2);
GLfloat lastY = static_cast<GLfloat>(screenHeight /2);

GLfloat deltaTime = 0.0f;	
GLfloat lastFrame = 0.0f;

float radCutOff = 0.0f;
float radOuterCutOff = 0.0f;

bool flashlight = false;

int main() {
	const int GLmajorVersion = 4;
	const int GLminorVersion = 4;

	Game game(
		"Coursework",
		1920, 1080,
		GLmajorVersion, GLminorVersion,
		false
	);

	while (!game.getWindowShouldClose()) {
		game.update();
		game.render();
	}

	//Window window("Coursework");
	//window.keyboardInput(key_callback);
	//window.mouseInput(mouse_callback);
	//window.scrolInput(scroll_callback);
	//
	//std::unique_ptr<Shader> lightingShader = std::make_unique<Shader>
	//	(GLmajorVersion, GLminorVersion, "objects.vs", "objects.frag");

	//std::unique_ptr<Shader> lampShader = std::make_unique<Shader>
	//	(GLmajorVersion, GLminorVersion, "lamp.vs", "lamp.frag");

	//Cube cube;
	//Quad quad;
	//Mesh testQuadMesh(quad,glm::vec3(-3.0f,-1.0f,0.0f));
	//Mesh testLamp(cube, glm::vec3(2.f, 2.f, -2.f));
	//Mesh testObj(cube);
	//testObj.setScale(glm::vec3(1.5f));

	//float vertices[] = {
	//	//	  Координаты			Нормали			Текстурные координаты
	//	//	// Задняя грань
	//	-0.5f, -0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		0.0f, 0.0f,
	//	 0.5f, -0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		1.0f, 0.0f,
	//	 0.5f,  0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		1.0f, 1.0f,
	//	 0.5f,  0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		1.0f, 1.0f,
	//	-0.5f,  0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		0.0f, 1.0f,
	//	-0.5f, -0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		0.0f, 0.0f,
	//	//	// Передняя грань
	//	-0.5f, -0.5f,  0.5f,	0.0f,  0.0f, 1.0f,		0.0f, 0.0f,
	//	-0.5f,  0.5f,  0.5f,	0.0f,  0.0f, 1.0f,		0.0f, 1.0f,
	//	 0.5f,  0.5f,  0.5f,	0.0f,  0.0f, 1.0f,		1.0f, 1.0f,
	//	 0.5f,  0.5f,  0.5f,	0.0f,  0.0f, 1.0f,		1.0f, 1.0f,
	//	 0.5f, -0.5f,  0.5f,	0.0f,  0.0f, 1.0f,		1.0f, 0.0f,
	//	-0.5f, -0.5f,  0.5f,	0.0f,  0.0f, 1.0f,		0.0f, 0.0f,
	//	//	// Левая грань
	//	-0.5f, -0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
	//	-0.5f,  0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,	0.0f, 1.0f,
	//	-0.5f,  0.5f,  0.5f,	-1.0f,  0.0f,  0.0f,	1.0f, 1.0f,
	//	-0.5f,  0.5f,  0.5f,	-1.0f,  0.0f,  0.0f,	1.0f, 1.0f,
	//	-0.5f, -0.5f,  0.5f,	-1.0f,  0.0f,  0.0f,	1.0f, 0.0f,
	//	-0.5f, -0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
	//	//	// Правая грань
	//	 0.5f,  0.5f,  0.5f,	1.0f,  0.0f,  0.0f,		1.0f, 1.0f,
	//	 0.5f,  0.5f, -0.5f,	1.0f,  0.0f,  0.0f,		0.0f, 1.0f,
	//	 0.5f, -0.5f, -0.5f,	1.0f,  0.0f,  0.0f,		0.0f, 0.0f,
	//	 0.5f, -0.5f, -0.5f,	1.0f,  0.0f,  0.0f,		0.0f, 0.0f,
	//	 0.5f, -0.5f,  0.5f,	1.0f,  0.0f,  0.0f,		1.0f, 0.0f,
	//	 0.5f,  0.5f,  0.5f,	1.0f,  0.0f,  0.0f,		1.0f, 1.0f,
	//	 //	 // Нижняя грань
	//	-0.5f, -0.5f, -0.5f,	0.0f, -1.0f,  0.0f,		0.0f, 1.0f,
	//	-0.5f, -0.5f,  0.5f,	0.0f, -1.0f,  0.0f,		0.0f, 0.0f,
	//	 0.5f, -0.5f,  0.5f,	0.0f, -1.0f,  0.0f,		1.0f, 0.0f,
	//	 0.5f, -0.5f,  0.5f,	0.0f, -1.0f,  0.0f,		1.0f, 0.0f,
	//	 0.5f, -0.5f, -0.5f,	0.0f, -1.0f,  0.0f,		1.0f, 1.0f,
	//	-0.5f, -0.5f, -0.5f,	0.0f, -1.0f,  0.0f,		0.0f, 1.0f,
	//	//	 // Верхняя грань
	//	-0.5f,  0.5f, -0.5f,	0.0f,  1.0f,  0.0f,		0.0f, 1.0f,
	//	 0.5f,  0.5f, -0.5f,	0.0f,  1.0f,  0.0f,		1.0f, 1.0f,
	//	 0.5f,  0.5f,  0.5f,	0.0f,  1.0f,  0.0f,		1.0f, 0.0f,
	//	 0.5f,  0.5f,  0.5f,	0.0f,  1.0f,  0.0f,		1.0f, 0.0f,
	//	-0.5f,  0.5f,  0.5f,	0.0f,  1.0f,  0.0f,		0.0f, 0.0f,
	//	-0.5f,  0.5f, -0.5f,	0.0f,  1.0f,  0.0f,		0.0f, 1.0f,
	//};

	//glm::vec3 pointLightColors[] = {
	//	glm::vec3(0.3f, 0.1f, 0.1f),
	//};

	//Texture diffuseMap("Images/container2.png", GL_TEXTURE_2D, 0);
	//Texture specularMap("Images/container2_specular.png", GL_TEXTURE_2D, 1);
	//Material material(diffuseMap.getUnit(), specularMap.getUnit(), 32.f);

	//while (!glfwWindowShouldClose(window.getWindowPtr())) {
	//	GLfloat currentFrame = glfwGetTime();
	//	deltaTime = currentFrame - lastFrame;
	//	lastFrame = currentFrame;
	//	window.processEvents();
	//	do_movement();

	//	glClearColor(0.f, 0.f, 0.f, 1.0f);
	//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	//	/*for (int i = 0; i < 4; ++i)
	//	{
	//		pointLightPositions[i].x = sin(glfwGetTime()) * 2.0f;
	//		pointLightPositions[i].y = cos(glfwGetTime() / 2.0f) * 1.0f;
	//		pointLightPositions[i].z = sin(glfwGetTime() / 2.0f) * 1.0f;
	//	}*/
	//	
	//	//LightningShader
	//	lightingShader.get()->Use();
	//	material.sendToShader(lightingShader.get());

	//	lightingShader.get()->setVec3("viewPos", camera.GetPoistion());


	//	//default
	//	lightingShader.get()->setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
	//	lightingShader.get()->setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
	//	lightingShader.get()->setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
	//	lightingShader.get()->setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
	//	for (int i = 0; i < 1; ++i) {
	//		std::string index = std::to_string(i);
	//		lightingShader.get()->setVec3("pointLights[" + index + "].position", testLamp.getPosition());
	//		lightingShader.get()->setVec3("pointLights[" + index + "].ambient", 0.05f, 0.05f, 0.05f);
	//		lightingShader.get()->setVec3("pointLights[" + index + "].diffuse", 0.8f, 0.8f, 0.8f);
	//		lightingShader.get()->setVec3("pointLights[" + index + "].specular", 1.0f, 1.0f, 1.0f);
	//		lightingShader.get()->setFloat("pointLights[" + index + "].constant", 1.0f);
	//		lightingShader.get()->setFloat("pointLights[" + index + "].linear", 0.09f);
	//		lightingShader.get()->setFloat("pointLights[" + index + "].quadratic", 0.032f);
	//	}
	//	lightingShader.get()->setVec3("spotLight.position", camera.GetPoistion());
	//	lightingShader.get()->setVec3("spotLight.direction", camera.GetFront());
	//	lightingShader.get()->setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
	//	lightingShader.get()->setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
	//	lightingShader.get()->setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
	//	lightingShader.get()->setFloat("spotLight.constant", 1.0f);
	//	lightingShader.get()->setFloat("spotLight.linear", 0.09f);
	//	lightingShader.get()->setFloat("spotLight.quadratic", 0.032f);

	//	////horror0.0
	//	//lightingShader.get()->setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
	//	//lightingShader.get()->setVec3("dirLight.ambient", 0.0f, 0.0f, 0.0f);
	//	//lightingShader.get()->setVec3("dirLight.diffuse", 0.05f, 0.05f, 0.05f);
	//	//lightingShader.get()->setVec3("dirLight.specular", 0.2f, 0.2f, 0.2f);
	//	//// Point light
	//	//lightingShader.get()->setVec3("pointLights[0].position", testLamp.getPosition());
	//	//lightingShader.get()->setVec3("pointLights[0].ambient", pointLightColors[0] * 0.1f);
	//	//lightingShader.get()->setVec3("pointLights[0].diffuse", pointLightColors[0]);
	//	//lightingShader.get()->setVec3("pointLights[0].specular", pointLightColors[0]);
	//	//lightingShader.get()->setFloat("pointLights[0].constant", 1.0f);
	//	//lightingShader.get()->setFloat("pointLights[0].linear", 0.14f);
	//	//lightingShader.get()->setFloat("pointLights[0].quadratic", 0.07f);
	//	//// SpotLight
	//	//lightingShader.get()->setVec3("spotLight.position",camera.GetPoistion());
	//	//lightingShader.get()->setVec3("spotLight.direction", camera.GetFront());
	//	//lightingShader.get()->setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
	//	//lightingShader.get()->setVec3("spotLight.diffuse", pointLightColors[0]);
	//	//lightingShader.get()->setVec3("spotLight.specular", pointLightColors[0]);
	//	//lightingShader.get()->setFloat("spotLight.constant", 1.0f);
	//	//lightingShader.get()->setFloat("spotLight.linear", 0.09f);
	//	//lightingShader.get()->setFloat("spotLight.quadratic", 0.032f);

	//	lightingShader.get()->setFloat("spotLight.cutOff", glm::cos(glm::radians(radCutOff)));
	//	lightingShader.get()->setFloat("spotLight.outerCutOff", glm::cos(glm::radians(radOuterCutOff)));

	//	glm::mat4 view;
	//	glm::mat4 projection;

	//	view = camera.GetViewMatrix();
	//	glfwGetFramebufferSize(window.getWindowPtr(), &framebufferWidth, &framebufferHeight);
	//	projection = glm::perspective(glm::radians(camera.GetZoom()), static_cast<GLfloat>(framebufferWidth) / static_cast<GLfloat>(framebufferHeight), 0.1f, 100.0f);

	//	lightingShader.get()->setMat4("view", view);
	//	lightingShader.get()->setMat4("projection", projection);

	//	diffuseMap.bindTexture();
	//	specularMap.bindTexture();

	//	testObj.move(glm::vec3(0.0001f, 0.f, 0.f));
	//	testObj.render(lightingShader.get());
	//	
	//	testQuadMesh.scaleUp(glm::vec3(0.00001f));
	//	testQuadMesh.render(lightingShader.get());

	//	glBindVertexArray(0);

	//	//Lamp shader
	//	lampShader.get()->Use();
	//	lampShader.get()->setMat4("view", view);
	//	lampShader.get()->setMat4("projection", projection);

	//	lampShader.get()->setVec3("lightColor", glm::vec3(1.f));

	//	testLamp.move(glm::vec3(-0.0001f,0.0f,0.0001f));

	//	testLamp.rotate(glm::vec3(0.001f));

	//	testLamp.render(lampShader.get());

	//	glBindVertexArray(0);

	//	glfwSwapBuffers(window.getWindowPtr());

	//	glUseProgram(0);
	//	glActiveTexture(0);
	//	glBindTexture(GL_TEXTURE_2D, 0);
	//}

	//

	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key == GLFW_KEY_F && action == GLFW_PRESS) {
		if (flashlight)
		{
			radCutOff = 0.0f;
			radOuterCutOff = 0.0f;
			flashlight = false;
		}
		else {
			radCutOff = 10.0f;
			radOuterCutOff = 15.0f;
			flashlight = true;
		}
	}
	if (action == GLFW_PRESS)
		keys[key] = true;
	else if (action == GLFW_RELEASE)
		keys[key] = false;
}

bool firstMouse = true;

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
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

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	camera.ProcessMouseScroll(yoffset);
}

void do_movement() {
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
