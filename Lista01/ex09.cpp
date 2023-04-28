#include <iostream>
#include <string>
#include <assert.h>
#include <vector>

using namespace std;

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double mouse_x, double mouse_y);

int drawRoof();
int drawWalls();
int drawDoor();
int drawWindow();
int drawSoil();

const GLuint WIDTH = 800, HEIGHT = 600;

int main()
{
	glfwInit();

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Casa", nullptr, nullptr);
	glfwMakeContextCurrent(window);


	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);


	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;

	}

	const GLubyte* renderer = glGetString(GL_RENDERER); 
	const GLubyte* version = glGetString(GL_VERSION);
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	Shader shader("HelloTriangle.vs", "HelloTriangle.fs");

	glUseProgram(shader.ID);
	
	GLuint roofVAO = drawRoof();
	GLuint wallsVAO = drawWalls();
	GLuint doorVAO = drawDoor();
	GLuint windowVAO = drawWindow();
	GLuint soilVAO = drawSoil();

	while (!glfwWindowShouldClose(window))
	{

		glfwPollEvents();

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// telhado
		glBindVertexArray(roofVAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// parede
		glLineWidth(5);
		glBindVertexArray(wallsVAO);
		glDrawArrays(GL_LINE_LOOP, 0, 4);

		// porta
		glLineWidth(5);
		glBindVertexArray(doorVAO);
		glDrawArrays(GL_LINE_LOOP, 0, 4);

		// janela
		glLineWidth(5);
		glBindVertexArray(windowVAO);
		glDrawArrays(GL_LINE_LOOP, 0, 4);

		// solo
		glLineWidth(5);
		glBindVertexArray(soilVAO);
		glDrawArrays(GL_LINES, 0, 2);

		glBindVertexArray(0); 


		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &roofVAO);

	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void mouse_callback(GLFWwindow* window, double mouse_x, double mouse_y)
{
	//cout << mouse_x << " " << mouse_y << endl;
}

void createBuffer(GLuint* VBO, GLuint* VAO, GLfloat* vertices, GLsizei size) {
	glGenBuffers(1, VBO);
	glBindBuffer(GL_ARRAY_BUFFER, *VBO);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, VAO);
	glBindVertexArray(*VAO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

int drawRoof() {
	GLfloat roof[] = {
		// x   y   z    r    g    b
		0.0, 0.5, 0.0, 1.0, 0.0, 0.0, //cima
		0.3, 0.2, 0.0, 1.0, 0.0, 0.0, //dir
		-0.3, 0.2, 0.0, 1.0, 0.0, 0.0 //esq
	};

	GLuint roofVBO, roofVAO;

	createBuffer(&roofVBO, &roofVAO, roof, sizeof(roof));

	return roofVAO;
}

int drawWalls() {
	GLfloat walls[] = {
		// x   y   z    r    g    b
		0.3, 0.2, 0.0, 0.0, 0.0, 0.0, //supdir
		-0.3, 0.2, 0.0, 0.0, 0.0, 0.0, //infdir
		-0.3, -0.6, 0.0, 0.0, 0.0, 0.0, //infesq
		0.3, -0.6, 0.0, 0.0, 0.0, 0.0 //supesq
	};

	GLuint wallsVBO, wallsVAO;

	createBuffer(&wallsVBO, &wallsVAO, walls, sizeof(walls));

	return wallsVAO;
}

int drawDoor() {
	GLfloat door[] = {
		// x   y   z    r    g    b
		0.1, -0.3, 0.0, 0.0, 0.0, 0.0, //supdir
		0.1, -0.6, 0.0, 0.0, 0.0, 0.0, //infdir
		-0.1, -0.6, 0.0, 0.0, 0.0, 0.0, //infesq
		-0.1, -0.3, 0.0, 0.0, 0.0, 0.0 //supesq
	};

	GLuint doorVBO, doorVAO;

	createBuffer(&doorVBO, &doorVAO, door, sizeof(door));

	return doorVAO;
}

int drawWindow() {
	GLfloat window[] = {
		// x   y   z    r    g    b
		-0.2, 0.0, 0.0, 0.0, 0.0, 0.0, //supdir
		-0.2, -0.1, 0.0, 0.0, 0.0, 0.0, //infdir
		-0.1, -0.1, 0.0, 0.0, 0.0, 0.0, //infesq
		-0.1, 0.0, 0.0, 0.0, 0.0, 0.0 //supesq
	};

	GLuint windowVBO, windowVAO;

	createBuffer(&windowVBO, &windowVAO, window, sizeof(window));

	return windowVAO;
}

int drawSoil() {
	GLfloat soil[] = {
		// x    y    z    r    g    b
		-1.0, -0.6, 0.0, 1.0, 0.5, 0.0,
		1.0, -0.6, 0.0, 1.0, 0.5, 0.0
	};

	GLuint soilVBO, soilVAO;

	createBuffer(&soilVBO, &soilVAO, soil, sizeof(soil));

	return soilVAO;
}