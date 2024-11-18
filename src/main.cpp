#include <iostream>
#include <Windows.h>
#include <cstdio>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.h"
#include "camera.h"

#include <stb/stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
unsigned int load_textures(std::string path);

// camera
Camera camera(glm::vec3(0.0f, 1.0f, 4.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;


int main(void)
{

	SetConsoleOutputCP(GetACP());
	stbi_set_flip_vertically_on_load(true);

	GLFWwindow* window;

	// DIRETIVAS PARA INICIALIZAR A JANELA
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Initialize the library */
	if (!glfwInit()) {
		return -1;
	}

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Hello World", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Specify the viewport of OpenGL in the Window
	// In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//faces dos cubos
	GLfloat vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, // back
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,//front
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, //left
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, // right
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, //bottom
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, //top
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};
	//posicoes dos varios cubos
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(0.0f,  1.0f, -1.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	glEnable(GL_DEPTH_TEST);

	glm::vec3 lightPos(1.2f, 1.0f, -1.0f);

	//inicalizando shader
	Shader shader("./assets/shaders/vertex_shader.glsl", "./assets/shaders/fragment_shader.glsl");
	Shader lightShader("./assets/shaders/light_vertex.glsl", "./assets/shaders/light_fragment.glsl");


	//VERTEX BUFFER OBJECT, VERTEX ARRAY OBJECT, ELEMENT BUFFER OBJECT
	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(VAO);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	GLuint lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glm::vec3 lightColor(1.0f, 1.0f, 0.0f);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


	// MODEL = MEU OBJETO, PROJECTION = TIPO DE PERSPECTIVA, VIEW = CAMERA
	double previousTime = glfwGetTime();
	int frameCount = 0;
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{

		double currentTime = glfwGetTime();
		frameCount++;
		// If a second has passed.
		if (currentTime - previousTime >= 1.0)
		{
			// Display the frame count here any way you want.
			std::cout << frameCount << std::endl;

			frameCount = 0;
			previousTime = currentTime;
		}

		// per-frame time logic
	   // --------------------
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		// input
		// -----
		processInput(window);

		/* Render here */
		// Clean the back buffer and assign the new color to it
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.use();
		shader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
		shader.setVec3("lightColor", lightColor);
		shader.setVec3("viewPos", camera.position);

		// PERSPECTIVA DA CAMERA
		glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		shader.setMat4("projection", projection);

		//VIEW = CAMERA
		glm::mat4 view = camera.getViewMatrix(); // camera view
		shader.setMat4("view", view);
		const float radius = 1.0f;
		float camX = sin(glfwGetTime()) * radius;
		float camY = cos(glfwGetTime()) * radius;
		float camZ = cos(glfwGetTime()) * radius;
		lightPos = glm::vec3(camX, 0.0f, camZ);

		glBindVertexArray(VAO);
		for (size_t i = 0; i < 2; i++)
		{

			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = (i) * 10.0f;
			model = glm::rotate(model, glm::radians(angle) * (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
			shader.setMat4("model", model);
			shader.setVec3("lightPos", lightPos);
			glDrawArrays(GL_TRIANGLES, 0, 36);

		}

		glBindVertexArray(0);
		lightShader.use();
		lightShader.setMat4("view", view);
		lightShader.setMat4("projection", projection);
		glm::mat4 model = glm::mat4(1.0f);

		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));
		lightShader.setMat4("model", model);
		lightShader.setVec3("lightColor", lightColor);
		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		/* Poll for and process events */
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &lightVAO);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	shader.deleteShader();

	glfwDestroyWindow(window);

	glfwTerminate();
	return 0;
}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
	glfwSetCursorPosCallback(window, mouse_callback);

	glfwSetScrollCallback(window, scroll_callback);

	float cameraSpeed = 2.5f * deltaTime;

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.processKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.processKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.processKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.processKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera.processKeyboard(UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		camera.processKeyboard(DOWN, deltaTime);


}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {

	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // ao contrario pq o eixo X é invertido

	lastX = xpos;
	lastY = ypos;

	camera.processMouseMovement(xoffset, yoffset);

}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll((float)yoffset);
}

unsigned int load_textures(std::string path) {
	//CARREGANDO TEXTURAS
	unsigned int texture;
	// texture 1
	// ---------
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	// The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		if (path.find(".png") != std::string::npos)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}

	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	return texture;

}