#include <glad/glad.h>
#include <glfw3.h>
#include <iostream>
#include "Shader.h"

void createVertexObjects(unsigned int* VAO, unsigned int* VBO);

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

void processInput(GLFWwindow *window);

// Settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

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

	unsigned int VAO, VBO;

	Shader shader("C:/Users/gusta/source/repos/First_OpenGL/First_OpenGL/shader_vs", "C:/Users/gusta/source/repos/First_OpenGL/First_OpenGL/shader_fs");

	createVertexObjects(&VAO, &VBO);

	// Bind the VAO before the render loop because we have only one VAO
	glBindVertexArray(VAO);

	// Same logic than VAO: we have only one shader program
	shader.use();

	// Render loop
	while (!glfwWindowShouldClose(window))
	{
		// Process all input
		processInput(window);

		// Clear actual buffer setting the color from glClearColor
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw the content specified in VAO
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// Swap buffers and poll for IO events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Deleting resources
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);

	// Unallocate all hints and clean resources made before
	glfwTerminate();

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
		// Postition		// Color
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
		0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f
	};

	// Generate and bind an Vertex Array Object with an ID (VAO)
	glGenVertexArrays(1, VAO);
	glBindVertexArray(*VAO);

	// Generate an Vertex Buffer Object with an ID (VBO)
	glGenBuffers(1, VBO);

	// Bind the array buffer with the VBO buffer and set the vertices in it
	glBindBuffer(GL_ARRAY_BUFFER, *VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Configure the positon attribute of the vertices in VAO
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * (sizeof(float)), (void*)0);
	glEnableVertexAttribArray(0);

	// Configure the color attribute of the vertices in VAO
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * (sizeof(float)), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// After configured, VAO and VBO can be unbinded
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
