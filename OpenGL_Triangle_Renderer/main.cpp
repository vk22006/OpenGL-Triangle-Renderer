#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

const int SCR_WIDTH = 650;
const int SCR_HEIGHT = 650;

// Shader programs
const char* vertexShaderSource = "#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main()\n"
	"{\n"
	"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0";
const char* fragmentShaderSource = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"   FragColor = vec4(1.1f, 0.8f, 0.2f, 1.0f);\n"    // Change the color of triangle here
	"}\n\0";

static void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

static void getOpenGLVerInfo() {
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

	GLFWwindow* window = glfwCreateWindow(650, 650, "OpenGL Triangle Renderer", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create new window";
		glfwTerminate();
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//Initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	return window;
}

static unsigned int CreateLinkShader() {
	//Vertex shader
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << "\n";
	}

	//Fragment shader
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::SHADER::COMPILATION_FAILED\n" << infoLog << "\n";
	}

	// Linking the shaders
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);     // Attaching vertex shader with shader program
	glAttachShader(shaderProgram, fragmentShader);   // Attaching fragment shader with shader program
	glLinkProgram(shaderProgram);                    // Linking the above attached sahders with the program

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::SHADER::LINKING_FAILED\n" << infoLog << "\n";
	}

	// Deleting indvitual shaders after linking
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

static unsigned int GenerateBindArrayBuffer(unsigned int* VBO) {
	//Stores in CPU
	const std::vector<float> vertices = {
		// x     y     z  
		-0.8f, -0.8f, 0.0f,
		0.8f, -0.8f, 0.0f,
		0.0f, 0.8f, 0.0f
	};

	//Transfer memory to GPU

	//Generate and bind VAO
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, VBO); // Use pointer to set VBO in main
	glBindBuffer(GL_ARRAY_BUFFER, *VBO);

	//Sending data and specifying its target, size and its usage
	glBufferData(
		GL_ARRAY_BUFFER,
		vertices.size() * sizeof(float),
		vertices.data(),
		GL_STATIC_DRAW
	);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	return VAO;
}

static void RenderLoop(GLFWwindow* window, unsigned int shaderProgram, unsigned int VAO) {

	while (!glfwWindowShouldClose(window)) {

		processInput(window);

		// render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// draw our first triangle
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}


int main() {

	GLFWwindow* window = Initialize();

	unsigned int shaderProgram = CreateLinkShader();

	unsigned int VBO; // Declare VBO
	unsigned int VAO = GenerateBindArrayBuffer(&VBO); // Pass VBO by pointer

	RenderLoop(window, shaderProgram, VAO);

	//Cleanup

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);
	glfwTerminate();

	return 0;
}