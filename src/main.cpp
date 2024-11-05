#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

//const char* vertexShaderSource = "#version 330 core\n"
//"layout (location = 0) in vec3 aPos;\n"
//"void main()\n"
//"{\n"
//"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
//"}\0";
//const char* fragmentShaderSource = "#version 330 core\n"
//"out vec4 FragColor;\n"
//"void main()\n"
//"{\n"
//"	FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
//"}\n\0";

static GLuint CompileShader(GLuint type, const std::string& source) {

	GLuint id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, NULL);
	glCompileShader(id);

	//TODO: Error handling
	GLint result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		GLint lenght;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &lenght);
		char* message = (char*)_malloca(lenght * sizeof(char));
		std::cout << "falhou " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << std::endl;
		std::cout << message << std::endl;

	}

	return id;
}

static GLuint CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {

	GLuint program = glCreateProgram();
	GLuint vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	GLuint fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

int main(void)
{
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
	window = glfwCreateWindow(800, 800, "Hello World", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	gladLoadGL();
	// Specify the viewport of OpenGL in the Window
	// In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
	glViewport(0, 0, 800, 800);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	GLfloat positions[6] = {
		-0.5f, -0.5f,
		 0.0f,  0.5f,
		 0.5f, -0.5f,
	};
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(GLfloat), positions, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 2, 0);

	std::string vertexShader =
		"#version 330 core\n"
		"\n"
		"layout (location = 0) in vec4 position;\n"
		"\n"
		"void main()\n"
		"{\n"
		"	gl_Position = position;\n"
		"}\n";

	std::string fragmentShader =
		"#version 330 core\n"
		"\n"
		"layout (location = 0) out vec4 color;\n"
		"\n"
		"void main()\n"
		"{\n"
		"	color = vec4(1.0, 0.0, 0.0, 1.0);\n"
		"}\n";
	GLuint shader = CreateShader(vertexShader, fragmentShader);
	glUseProgram(shader);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		///* Render here */
		// Clean the back buffer and assign the new color to it
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}


	glDeleteProgram(shader);

	glfwDestroyWindow(window);

	glfwTerminate();
	return 0;
}