#include <glad/glad.h>
#include <glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
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
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "And then I saw her face", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Failed to generate GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Make the context/state of the window created before the actual context
	glfwMakeContextCurrent(window);

	// Initialize GLAD to use functions OS-specific for OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// Set the OpenGL Viewport to the (not necessarely) same as the GLFW
	glViewport(0, 0, 800, 600);

	// Set callback function for resize the window
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Render loop
	while (!glfwWindowShouldClose(window))
	{
		// Process all input
		processInput(window);

		// Clear actual buffer setting the color from glClearColor
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Swap buffers and poll IO events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Unallocate all hints and clean resources made before
	glfwTerminate();

	return 0;
}

// Callback function: resizing the window should call this function
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// Process all input
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}