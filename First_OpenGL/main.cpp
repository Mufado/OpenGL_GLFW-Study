#include <glad/glad.h>
#include <glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void createVertexObjects(unsigned int* VAO, unsigned int* VBO);
void testShader(GLuint vertexShader);
void testProgram(GLuint program);
unsigned int createShaderProgram();
void terminate(unsigned int shaderProgram);

// Settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Vertex shader in shading language
const char *vertexShaderSource =
	"#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main()\n"
	"{\n"
	"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0";

const char* fragmentShaderSource = 
	"#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"}\n\0";

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); --> For MAC OS systems

	// Create window object that is necessary to use GLFW functions and render the image
	GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "And then I saw her face", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Failed to generate GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Make the context/state of the window created before the actual context
	glfwMakeContextCurrent(window);

	// Set callback function for resize the window
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Initialize GLAD to use functions OS-specific for OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	unsigned int VAO, VBO, shaderProgram;

	shaderProgram = createShaderProgram();
	createVertexObjects(&VAO, &VBO);

	// Render loop
	while (!glfwWindowShouldClose(window))
	{
		// Process all input
		processInput(window);

		// Clear actual buffer setting the color from glClearColor
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw the content specified in VAO
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// Swap buffers and poll for IO events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	terminate(shaderProgram);

	return 0;
}

// Callback function: resizing the window should call this function
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// Process all input
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}

// Set the vertices for draw a triangle in the GPU memory
void createVertexObjects(unsigned int *VAO, unsigned int *VBO)
{
	// Set the postition of the vertices in NDC pattern
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f,  0.5f, 0.0f
	};

	// Generate and bind an Vertex Array Object with an ID (VAO)
	glGenVertexArrays(1, VAO);
	glBindVertexArray(*VAO);

	// Generate an Vertex Buffer Object with an ID (VBO)
	glGenBuffers(1, VBO);

	// Bind the array buffer with the VBO buffer and set the vertices in it
	glBindBuffer(GL_ARRAY_BUFFER, *VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Configure how OpenGL interpret the VAO
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * (sizeof(float)), (void*)0);
	glEnableVertexAttribArray(0);

	// After configured, VAO and VBO can be unbinded
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

// Create and link a Shader Program
unsigned int createShaderProgram()
{
	// Create a vertex shader and compiles using the vertexShaderSource
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	testShader(vertexShader);

	// Create the Fragment Shader with an ID
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	testShader(fragmentShader);

	// Create and link the Shader Program with both compiled shaders
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// Delete shader allocation
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

// Test if shader compilation worked
void testShader(GLuint shader)
{
	int  success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
}

// Test if program creation worked
void testProgram(GLuint program)
{
	int  success;
	char infoLog[512];
	glGetProgramiv(program, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::CREATION_FAILED\n" << infoLog << std::endl;
	}
}

// Terminate the program and delete all resources
void terminate(unsigned int shaderProgram)
{
	// Unallocate all hints and clean resources made before
	glfwTerminate();

	// Deleting resources
	glDeleteProgram(shaderProgram);
}