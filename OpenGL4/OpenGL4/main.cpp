#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <sstream>
#include <streambuf>
#include <string>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
std::string loadShaderSrc(const char* filename);

int main() {

	std::cout << "Hello" << std::endl;
	int success;
	char infoLof[512];

	glfwInit();
	// version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL window", NULL, NULL);
	if (window == NULL) {
		std::cout << "Could not create window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// GLAD codes
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Faild to init GLAD" << std::endl;
		glfwTerminate();
		return -1;
	}
	//setup Viewport
	glViewport(0,0,800,600);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	/*
		shaders
	*/

	// compile vertex shader
	unsigned int vertextShader;
	vertextShader = glCreateShader(GL_VERTEX_SHADER);
	std::string vertSharderSrc = loadShaderSrc("assets/vertext_core.glsl");
	const GLchar* vertShader = vertSharderSrc.c_str();
	glShaderSource(vertextShader, 1, &vertShader, NULL);
	glCompileShader(vertextShader);

	//catch error
	glGetShaderiv(vertextShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertextShader, 512, NULL, infoLof);
		std::cout << "Error with vertext shader comp." << std::endl << infoLof << std::endl;

	}

	//compile fragment shader
	unsigned int fragmentShader[2];
	fragmentShader[0] = glCreateShader(GL_FRAGMENT_SHADER);
	std::string fragSgaderSrc = loadShaderSrc("assets/fragment_core.glsl");
	const GLchar* fragShader = fragSgaderSrc.c_str();
	glShaderSource(fragmentShader[0], 1, &fragShader, NULL);
	glCompileShader(fragmentShader[0]);

	//catch error
	glGetShaderiv(fragmentShader[0], GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader[0], 512, NULL, infoLof);
		std::cout << "Error with fragment shader comp." << std::endl << infoLof << std::endl;

	}

	//compile fragment shader 2
	fragmentShader[1] = glCreateShader(GL_FRAGMENT_SHADER);
	fragSgaderSrc = loadShaderSrc("assets/fragment_core2.glsl");
	fragShader = fragSgaderSrc.c_str();
	glShaderSource(fragmentShader[1], 1, &fragShader, NULL);
	glCompileShader(fragmentShader[1]);

	//catch error
	glGetShaderiv(fragmentShader[1], GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader[1], 512, NULL, infoLof);
		std::cout << "Error with fragment shader comp." << std::endl << infoLof << std::endl;

	}

	unsigned int  shaderProgram[2];
	shaderProgram[0] = glCreateProgram();

	glAttachShader(shaderProgram[0], vertextShader);
	glAttachShader(shaderProgram[0], fragmentShader[0]);
	glLinkProgram(shaderProgram[0]);

	//catch error
	glGetProgramiv(shaderProgram[0], GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram[0], 512, NULL, infoLof);
		std::cout << "Error with shader1 program." << std::endl << infoLof << std::endl;

	}

	shaderProgram[1] = glCreateProgram();

	glAttachShader(shaderProgram[1], vertextShader);
	glAttachShader(shaderProgram[1], fragmentShader[1]);
	glLinkProgram(shaderProgram[1]);

	//catch error
	glGetProgramiv(shaderProgram[1], GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram[1], 512, NULL, infoLof);
		std::cout << "Error with shader2 program." << std::endl << infoLof << std::endl;

	}

	glDeleteShader(vertextShader);
	glDeleteShader(fragmentShader[0]);
	glDeleteShader(fragmentShader[1]);

	//vertext array
	//float vertices[] = {
	//	 0.5f,0.5f,0.0f,//top right
	//	-0.5f,0.5f,0.0f, // top left
	//	-0.5f,-0.5f,0.0f, //bottom left
	//	 0.5f,-0.5f,0.0f,//bottom right
	//};
	//
	//unsigned int indeices[] = {
	//	0,1,2, // first tri
	//	2,3,0 // second tri
	//};

	float vertices[] = {
		-0.5f,-0.5f,0.0f, //0
		-0.25f,0.5f,0.0f, //1
		-0.1f,-0.5f,0.0f, //2

		0.5f,-0.5f,0.0f, //3
		0.25f,0.5f,0.0f, //4
		0.1f,-0.5f,0.0f, //5
	};
	unsigned int indeices[] = {
		0,4,2,
		3,1,5

	};

	//VAO,VBO,EBO
	unsigned int VAO, VBO,EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	//bind VAO
	glBindVertexArray(VAO);

	// bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),vertices,GL_STATIC_DRAW);

	// set attribute pointer
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//set up EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indeices), indeices, GL_STATIC_DRAW);

	while (!glfwWindowShouldClose(window)) {
		processInput(window);
		//render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//draw shaps
		glBindVertexArray(VAO);
		//first triagnle
		glUseProgram(shaderProgram[0]);
		//glDrawArrays(GL_TRIANGLES, 0, 6);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

		//second triagnel
		glUseProgram(shaderProgram[1]);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(3*sizeof(unsigned int)));

		// send new frame to window
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glfwTerminate();

	return 0;
}
// for resize window
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

std::string loadShaderSrc(const char* filename) {
	std::ifstream file;
	std::stringstream buf;

	std::string ret = "";

	file.open(filename);
	if (file.is_open()) {
		buf << file.rdbuf();
		ret = buf.str();
	}
	else {
		std::cout << "Could not open " << filename << std::endl;
	}

	file.close();
	return ret;
}