#include "libs.h"
#include "Window.h"
#include <array>

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

void do_movement();

int screenWidth = 1920;
int screenHeight = 1080;
int framebufferWidth = 1920;
int framebufferHeight = 1080;
Camera camera(glm::vec3(0.0f, 0.0f, 6.0f));


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
	Window window("Coursework");
	window.keyboardInput(key_callback);
	window.mouseInput(mouse_callback);
	window.scrolInput(scroll_callback);

	std::unique_ptr<Shader> lightingShader = std::make_unique<Shader>
		("objects.vs", "objects.frag");
	std::unique_ptr<Shader> lampShader = std::make_unique<Shader>
		("lamp.vs", "lamp.frag");

	Vertex testVertices[] = {
		// Задняя грань (красная)
		{{-0.5f, -0.5f, -0.5f}, {0.0f,  0.0f, -1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
		{{ 0.5f, -0.5f, -0.5f}, {0.0f,  0.0f, -1.0f}, {1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
		{{ 0.5f,  0.5f, -0.5f}, {0.0f,  0.0f, -1.0f}, {1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},
		{{ 0.5f,  0.5f, -0.5f}, {0.0f,  0.0f, -1.0f}, {1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},
		{{-0.5f,  0.5f, -0.5f}, {0.0f,  0.0f, -1.0f}, {0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},
		{{-0.5f, -0.5f, -0.5f}, {0.0f,  0.0f, -1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},

		// Передняя грань (зеленая)
		{{-0.5f, -0.5f,  0.5f}, {0.0f,  0.0f,  1.0f}, {0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
		{{-0.5f,  0.5f,  0.5f}, {0.0f,  0.0f,  1.0f}, {0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},
		{{ 0.5f,  0.5f,  0.5f}, {0.0f,  0.0f,  1.0f}, {1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},
		{{ 0.5f,  0.5f,  0.5f}, {0.0f,  0.0f,  1.0f}, {1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},
		{{ 0.5f, -0.5f,  0.5f}, {0.0f,  0.0f,  1.0f}, {1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
		{{-0.5f, -0.5f,  0.5f}, {0.0f,  0.0f,  1.0f}, {0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},

		// Левая грань (синяя)
		{{-0.5f, -0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
		{{-0.5f,  0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f}, {0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
		{{-0.5f,  0.5f,  0.5f}, {-1.0f,  0.0f,  0.0f}, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
		{{-0.5f,  0.5f,  0.5f}, {-1.0f,  0.0f,  0.0f}, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
		{{-0.5f, -0.5f,  0.5f}, {-1.0f,  0.0f,  0.0f}, {1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
		{{-0.5f, -0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},

		// Правая грань (желтая)
		{{ 0.5f,  0.5f,  0.5f}, {1.0f,  0.0f,  0.0f}, {1.0f, 1.0f}, {1.0f, 1.0f, 0.0f}},
		{{ 0.5f,  0.5f, -0.5f}, {1.0f,  0.0f,  0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f, 0.0f}},
		{{ 0.5f, -0.5f, -0.5f}, {1.0f,  0.0f,  0.0f}, {0.0f, 0.0f}, {1.0f, 1.0f, 0.0f}},
		{{ 0.5f, -0.5f, -0.5f}, {1.0f,  0.0f,  0.0f}, {0.0f, 0.0f}, {1.0f, 1.0f, 0.0f}},
		{{ 0.5f, -0.5f,  0.5f}, {1.0f,  0.0f,  0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f, 0.0f}},
		{{ 0.5f,  0.5f,  0.5f}, {1.0f,  0.0f,  0.0f}, {1.0f, 1.0f}, {1.0f, 1.0f, 0.0f}},

		// Нижняя грань (оранжевая)
		{{-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f,  0.0f}, {0.0f, 1.0f}, {1.0f, 0.5f, 0.0f}},
		{{-0.5f, -0.5f,  0.5f}, {0.0f, -1.0f,  0.0f}, {0.0f, 0.0f}, {1.0f, 0.5f, 0.0f}},
		{{ 0.5f, -0.5f,  0.5f}, {0.0f, -1.0f,  0.0f}, {1.0f, 0.0f}, {1.0f, 0.5f, 0.0f}},
		{{ 0.5f, -0.5f,  0.5f}, {0.0f, -1.0f,  0.0f}, {1.0f, 0.0f}, {1.0f, 0.5f, 0.0f}},
		{{ 0.5f, -0.5f, -0.5f}, {0.0f, -1.0f,  0.0f}, {1.0f, 1.0f}, {1.0f, 0.5f, 0.0f}},
		{{-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f,  0.0f}, {0.0f, 1.0f}, {1.0f, 0.5f, 0.0f}},

		// Верхняя грань (пурпурная)
		{{-0.5f,  0.5f, -0.5f}, {0.0f,  1.0f,  0.0f}, {0.0f, 1.0f}, {0.5f, 0.0f, 0.5f}},
		{{ 0.5f,  0.5f, -0.5f}, {0.0f,  1.0f,  0.0f}, {1.0f, 1.0f}, {0.5f, 0.0f, 0.5f}},
		{{ 0.5f,  0.5f,  0.5f}, {0.0f,  1.0f,  0.0f}, {1.0f, 0.0f}, {0.5f, 0.0f, 0.5f}},
		{{ 0.5f,  0.5f,  0.5f}, {0.0f,  1.0f,  0.0f}, {1.0f, 0.0f}, {0.5f, 0.0f, 0.5f}},
		{{-0.5f,  0.5f,  0.5f}, {0.0f,  1.0f,  0.0f}, {0.0f, 0.0f}, {0.5f, 0.0f, 0.5f}},
		{{-0.5f,  0.5f, -0.5f}, {0.0f,  1.0f,  0.0f}, {0.0f, 1.0f}, {0.5f, 0.0f, 0.5f}},
	};

	Mesh test(testVertices, 36, nullptr, 0,glm::vec3(2.f,2.f,-2.f));
	Mesh test1(testVertices, 36, nullptr, 0);
	test1.setScale(glm::vec3(1.5f));

	float vertices[] = {
		//	  Координаты			Нормали			Текстурные координаты
		//	// Задняя грань
		-0.5f, -0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		0.0f, 0.0f,
		//	// Передняя грань
		-0.5f, -0.5f,  0.5f,	0.0f,  0.0f, 1.0f,		0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,	0.0f,  0.0f, 1.0f,		0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,	0.0f,  0.0f, 1.0f,		1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,	0.0f,  0.0f, 1.0f,		1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,	0.0f,  0.0f, 1.0f,		1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,	0.0f,  0.0f, 1.0f,		0.0f, 0.0f,
		//	// Левая грань
		-0.5f, -0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,	0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,	-1.0f,  0.0f,  0.0f,	1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,	-1.0f,  0.0f,  0.0f,	1.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,	-1.0f,  0.0f,  0.0f,	1.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
		//	// Правая грань
		 0.5f,  0.5f,  0.5f,	1.0f,  0.0f,  0.0f,		1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,	1.0f,  0.0f,  0.0f,		0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,	1.0f,  0.0f,  0.0f,		0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,	1.0f,  0.0f,  0.0f,		0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,	1.0f,  0.0f,  0.0f,		1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,	1.0f,  0.0f,  0.0f,		1.0f, 1.0f,
		 //	 // Нижняя грань
		-0.5f, -0.5f, -0.5f,	0.0f, -1.0f,  0.0f,		0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,	0.0f, -1.0f,  0.0f,		0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,	0.0f, -1.0f,  0.0f,		1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,	0.0f, -1.0f,  0.0f,		1.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,	0.0f, -1.0f,  0.0f,		1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,	0.0f, -1.0f,  0.0f,		0.0f, 1.0f,
		//	 // Верхняя грань
		-0.5f,  0.5f, -0.5f,	0.0f,  1.0f,  0.0f,		0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,	0.0f,  1.0f,  0.0f,		1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,	0.0f,  1.0f,  0.0f,		1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,	0.0f,  1.0f,  0.0f,		1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,	0.0f,  1.0f,  0.0f,		0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,	0.0f,  1.0f,  0.0f,		0.0f, 1.0f,
	};

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};

	glm::vec3 pointLightColors[] = {
		glm::vec3(0.3f, 0.1f, 0.1f),
		glm::vec3(0.3f, 0.1f, 0.1f),
		glm::vec3(0.3f, 0.1f, 0.1f),
		glm::vec3(0.3f, 0.1f, 0.1f)
	};

	GLuint VBO; 
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	GLuint lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	Texture diffuseMap("Images/container2.png", GL_TEXTURE_2D, 0);
	Texture specularMap("Images/container2_specular.png", GL_TEXTURE_2D, 1);
	Material material(diffuseMap.getUnit(), specularMap.getUnit(), 32.f);

	while (!glfwWindowShouldClose(window.getWindowPtr())) {
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		window.processEvents();
		do_movement();

		glClearColor(0.f, 0.f,0.f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		/*for (int i = 0; i < 4; ++i)
		{
			pointLightPositions[i].x = sin(glfwGetTime()) * 2.0f;
			pointLightPositions[i].y = cos(glfwGetTime() / 2.0f) * 1.0f;
			pointLightPositions[i].z = sin(glfwGetTime() / 2.0f) * 1.0f;
		}*/
		
		//LightningShader
		lightingShader.get()->Use();

		material.sendToShader(lightingShader.get());

		lightingShader.get()->setVec3("viewPos", camera.GetPoistion());


		//default
		/*lightingShader.get()->setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
		lightingShader.get()->setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
		lightingShader.get()->setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
		lightingShader.get()->setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
		for (int i = 0; i < 4; ++i) {
			std::string index = std::to_string(i);
			lightingShader.get()->setVec3("pointLights[" + index + "].position", pointLightPositions[i]);
			lightingShader.get()->setVec3("pointLights[" + index + "].ambient", 0.05f, 0.05f, 0.05f);
			lightingShader.get()->setVec3("pointLights[" + index + "].diffuse", 0.8f, 0.8f, 0.8f);
			lightingShader.get()->setVec3("pointLights[" + index + "].specular", 1.0f, 1.0f, 1.0f);
			lightingShader.get()->setFloat("pointLights[" + index + "].constant", 1.0f);
			lightingShader.get()->setFloat("pointLights[" + index + "].linear", 0.09f);
			lightingShader.get()->setFloat("pointLights[" + index + "].quadratic", 0.032f);
		}
		lightingShader.get()->setVec3("spotLight.position", camera.GetPoistion());
		lightingShader.get()->setVec3("spotLight.direction", camera.GetFront());
		lightingShader.get()->setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
		lightingShader.get()->setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
		lightingShader.get()->setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
		lightingShader.get()->setFloat("spotLight.constant", 1.0f);
		lightingShader.get()->setFloat("spotLight.linear", 0.09f);
		lightingShader.get()->setFloat("spotLight.quadratic", 0.032f);*/

		//horror
		lightingShader.get()->setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
		lightingShader.get()->setVec3("dirLight.ambient", 0.0f, 0.0f, 0.0f);
		lightingShader.get()->setVec3("dirLight.diffuse", 0.05f, 0.05f, 0.05f);
		lightingShader.get()->setVec3("dirLight.specular", 0.2f, 0.2f, 0.2f);
		// Point light 1
		lightingShader.get()->setVec3("pointLights[0].position", pointLightPositions[0]);
		lightingShader.get()->setVec3("pointLights[0].ambient", pointLightColors[0] * 0.1f);
		lightingShader.get()->setVec3("pointLights[0].diffuse", pointLightColors[0]);
		lightingShader.get()->setVec3("pointLights[0].specular", pointLightColors[0]);
		lightingShader.get()->setFloat("pointLights[0].constant", 1.0f);
		lightingShader.get()->setFloat("pointLights[0].linear", 0.14f);
		lightingShader.get()->setFloat("pointLights[0].quadratic", 0.07f);
		// Point light 2
		lightingShader.get()->setVec3("pointLights[1].position", pointLightPositions[1]);
		lightingShader.get()->setVec3("pointLights[1].ambient", pointLightColors[1] * 0.1f);
		lightingShader.get()->setVec3("pointLights[1].diffuse", pointLightColors[1]);
		lightingShader.get()->setVec3("pointLights[1].specular", pointLightColors[1]);
		lightingShader.get()->setFloat("pointLights[1].constant", 1.0f);
		lightingShader.get()->setFloat("pointLights[1].linear", 0.14f);
		lightingShader.get()->setFloat("pointLights[1].quadratic", 0.07f);
		// Point light 3
		lightingShader.get()->setVec3("pointLights[2].position", pointLightPositions[2]);
		lightingShader.get()->setVec3("pointLights[2].ambient", pointLightColors[2] * 0.1f);
		lightingShader.get()->setVec3("pointLights[2].diffuse", pointLightColors[2]);
		lightingShader.get()->setVec3("pointLights[2].specular", pointLightColors[2]);
		lightingShader.get()->setFloat("pointLights[2].constant", 1.0f);
		lightingShader.get()->setFloat("pointLights[2].linear", 0.22f);
		lightingShader.get()->setFloat("pointLights[2].quadratic", 0.20f);
		// Point light 4
		lightingShader.get()->setVec3("pointLights[3].position", pointLightPositions[3]);
		lightingShader.get()->setVec3("pointLights[3].ambient", pointLightColors[3] * 0.1f);
		lightingShader.get()->setVec3("pointLights[3].diffuse", pointLightColors[3]);
		lightingShader.get()->setVec3("pointLights[3].specular", pointLightColors[3]);
		lightingShader.get()->setFloat("pointLights[3].constant", 1.0f);
		lightingShader.get()->setFloat("pointLights[3].linear", 0.14f);
		lightingShader.get()->setFloat("pointLights[3].quadratic", 0.07f);
		// SpotLight
		lightingShader.get()->setVec3("spotLight.position",camera.GetPoistion());
		lightingShader.get()->setVec3("spotLight.direction", camera.GetFront());
		lightingShader.get()->setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
		lightingShader.get()->setVec3("spotLight.diffuse", pointLightColors[0]);
		lightingShader.get()->setVec3("spotLight.specular", pointLightColors[0]);
		lightingShader.get()->setFloat("spotLight.constant", 1.0f);
		lightingShader.get()->setFloat("spotLight.linear", 0.09f);
		lightingShader.get()->setFloat("spotLight.quadratic", 0.032f);

		lightingShader.get()->setFloat("spotLight.cutOff", glm::cos(glm::radians(radCutOff)));
		lightingShader.get()->setFloat("spotLight.outerCutOff", glm::cos(glm::radians(radOuterCutOff)));

		glm::mat4 model(1.f);
		glm::mat4 view;
		glm::mat4 projection;
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
		view = camera.GetViewMatrix();
		glfwGetFramebufferSize(window.getWindowPtr(), &framebufferWidth, &framebufferHeight);
		projection = glm::perspective(glm::radians(camera.GetZoom()), static_cast<GLfloat>(framebufferWidth) / static_cast<GLfloat>(framebufferHeight), 0.1f, 100.0f);

		lightingShader.get()->setMat4("view", view);
		lightingShader.get()->setMat4("projection", projection);

		diffuseMap.bindTexture();
		specularMap.bindTexture();

		test1.move(glm::vec3(0.0001f, 0.f, 0.f));
		test.render(lightingShader.get());
		test1.render(lightingShader.get());

		glBindVertexArray(0);

		//Lamp shader
		lampShader.get()->Use();
		lampShader.get()->setMat4("view", view);
		lampShader.get()->setMat4("projection", projection);

		glBindVertexArray(lightVAO);
		for (unsigned int i = 0; i < 4; i++)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.1f));
			lampShader.get()->setVec3("lightColor", pointLightColors[i]);
			lampShader.get()->setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glBindVertexArray(0);

		glfwSwapBuffers(window.getWindowPtr());

		glUseProgram(0);
		glActiveTexture(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);

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
