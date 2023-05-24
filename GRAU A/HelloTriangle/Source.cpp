#include <iostream>
#include <string>
#include <assert.h>

using namespace std;

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "Shader.h"
#include "stb_image.h"
//#include "Sprite.h"
#include "Meteor.h"
#include "Timer.h"


// Prot�tipo da fun��o de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double mouse_x, double mouse_y);

// Prot�tipos das fun��es
int setupTexture(string texName, int& width, int& height);
bool testCollision(Sprite &a, Sprite &b);

// Dimens�es da janela (pode ser alterado em tempo de execu��o)
const GLuint WIDTH = 800, HEIGHT = 600;

//Sprites globais
Sprite player;

// Fun��o MAIN
int main()
{
	srand(glfwGetTime());

	glfwInit();
	
	// Cria��o da janela GLFW
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Ola Sprites!!!", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Fazendo o registro da fun��o de callback para a janela GLFW
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);


	// GLAD: carrega todos os ponteiros d fun��es da OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;

	}

	// Obtendo as informa��es de vers�o
	const GLubyte* renderer = glGetString(GL_RENDERER); /* get renderer string */
	const GLubyte* version = glGetString(GL_VERSION); /* version as a string */
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;

	// Compilando e buildando o programa de shader
	Shader shader("HelloTriangle.vs", "HelloTriangle.fs");

	// Textura
	int texWidth, texHeight;
	GLuint texID = setupTexture("../../Textures/background.png", texWidth, texHeight);
	
	// Fundo
	Sprite background;
	background.initialize(texID, glm::vec2(texWidth, texHeight), &shader);

	texID = setupTexture("../../Textures/laser.png", texWidth, texHeight);
	Meteor laser;
	laser.initialize(texID, glm::vec2(texWidth*2, texHeight * 2), &shader, 1, 3, glm::vec3(400.0,620.0,0.0));
	laser.setVelocity(10.0);

	texID = setupTexture("../../Textures/ship.png", texWidth, texHeight);
	player.initialize(texID, glm::vec2(texWidth * 0.1, texHeight  *0.1), &shader, 1, 3, glm::vec3(100.0,150.0,0.0));
	player.setVelocity(7.5);

	Timer timer;

	glUseProgram(shader.ID);

	glActiveTexture(GL_TEXTURE0);
	shader.setTexBuffer0("texBuff");

	glm::mat4 projection = glm::ortho(0.0, 800.0, 0.0, 600.0, -1.0, 1.0);
	
	GLint projLoc = glGetUniformLocation(shader.ID, "projection");
	glUniformMatrix4fv(projLoc, 1, FALSE, glm::value_ptr(projection));

	//Habilitando a fun��o de mistura no color buffer - transpar�ncia
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Habilita o teste de profundidade
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_ALWAYS);


	// Loop da aplica��o - "game loop"
	while (!glfwWindowShouldClose(window))
	{
		timer.start();
		
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);

		glfwPollEvents();

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //cor de fundo
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		background.update();
		background.draw();

		laser.update();
		laser.draw();

		player.update();
		player.draw();

		bool isColliding = testCollision(laser, player);
		//AQUI processa a l�gica da colis�o

		//cout << player.getPosition().x << " " << player.getPosition().y << endl;

		timer.finish();
		double waitingTime = timer.calcWaitingTime(30, timer.getElapsedTimeMs());
		if (waitingTime > 0)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds((int)waitingTime));
		}

		// Troca os buffers da tela
		glfwSwapBuffers(window);

	}
	
	// Finaliza a execu��o da GLFW, limpando os recursos alocados por ela
	glfwTerminate();
	return 0;
}

// Fun��o de callback de teclado - s� pode ter uma inst�ncia (deve ser est�tica se
// estiver dentro de uma classe) - � chamada sempre que uma tecla for pressionada
// ou solta via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_D || key == GLFW_KEY_RIGHT)
	{
		player.moveRight();
	}

	if (key == GLFW_KEY_A || key == GLFW_KEY_LEFT)
	{
		player.moveLeft();
	}

}

void mouse_callback(GLFWwindow* window, double mouse_x, double mouse_y)
{
	//cout << mouse_x << " " << mouse_y << endl;
}

int setupTexture(string texName, int& width, int& height)
{
	GLuint texID;

	// Gera o identificador da textura na mem�ria 
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	//Parametrizando o wrapping da textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


	//Carregamento do arquivo de imagem da textura
	int nrChannels;
	unsigned char* data = stbi_load(texName.c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		//Enviar a informa��o dos pixels da imagem para OpenGL gerar o buffer de textura
		if (nrChannels == 3) //jpg, bmp
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else //png
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	return texID;
}

bool testCollision(Sprite &a, Sprite &b)
{
	AABB pA = a.getAABB();
	AABB pb = b.getAABB();

	//teste de colis�o
	//Fazer o if pra ver se um AABB n�o sobrep�e o outro
	
	return true;
}
