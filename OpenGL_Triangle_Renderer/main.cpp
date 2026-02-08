#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

const int SCR_WIDTH = 650;
const int SCR_HEIGHT = 650;

void getOpenGLVerInfo() {
	std::cout << "Vendor: " << glGetString(GL_VENDOR) << "\n";
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << "\n";
	std::cout << "Version: " << glGetString(GL_VERSION) << "\n";
	std::cout << "Shading language: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";
}

static GLFWwindow* Initialize() {
	//Initialize glfw
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(650, 650, "My First OpenGL", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create new window";
		glfwTerminate();
	}

	glfwMakeContextCurrent(window);

	//Initialize GLAD
	gladLoadGL();
	glViewport(0, 0, 650, 650);
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glfwSwapBuffers(window);

	//OpenGL version info using glGetString()
	//getOpenGLVerInfo();

	return window;
}

void VertexSpecifications() {

	//Stores in CPU
	const std::vector<float> vertices = {
		// x     y     z  
		-0.8f, -0.8f, 0.0f,
		0.8f, -0.8f, 0.0f,
		0.8f, 0.8f, 0.0f
	};

	
}

void CreateGraphicsPipeline() {}

void RenderLoop(GLFWwindow* window) {
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
	}
}

void CleanClose(GLFWwindow* window) {
	glfwDestroyWindow(window);
	glfwTerminate();
}

int main() {

	GLFWwindow* window = Initialize();

	VertexSpecifications();

	CreateGraphicsPipeline();

	RenderLoop(window);

	CleanClose(window);
	
	return 0;
}