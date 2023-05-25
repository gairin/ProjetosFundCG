#include "Meteor.h"

void Meteor::update()
{
	// atualiza a seed de randomiza��o
	std::srand(static_cast<unsigned int>(std::time(nullptr)));

	// saiu da tela, respawna outro, conta ponto
	if (position.y < 0 || position.y > 620 || position.x < 0 || position.x > 800)
	{	
		// gera aleatoriamente um modo de disparo (dire��es horizontais e verticais)
		m = (rand() % 4);
		respawn(m);
	}

	switch (m) {
		case 0:
			position.y -= vel;
			angle = 0;
			break;
		case 1:
			position.y += vel;
			angle = 0;
			break;
		case 2:
			position.x += vel;
			angle = glm::radians(90.0f);
			break;
		case 3:
			position.x -= vel;
			angle = glm::radians(90.0f);
			break;
	}

	//Matriz de modelo: transforma��es no objeto
	glm::mat4 model = glm::mat4(1); //matriz identidade
	model = glm::translate(model, position);
	model = glm::rotate(model, angle, axis);
	model = glm::scale(model, scale);

	//Enviando a matriz de modelo para o shader
	shader->setMat4("model", glm::value_ptr(model));

	//Enviando para o shader o deslocamento das coords de textura
	//de acordo com os indices de anima��o e frame
	iFrame = (iFrame + 1) % nFrames;
	shader->setVec2("offsets", iFrame * dx, iAnimation * dy);

}

void Meteor::respawn(int m) {
	switch(m) {
		// de cima pra baixo
		case 0:
			// (padr�o)
			position.y = 620;
			position.x = rand() % 800;
			break;
		// de baixo pra cima
		case 1:
			position.y = 0;
			position.x = rand() % 800;
			break;
		// da esquerda pra direita
		case 2:
			position.y = rand() % 620;
			position.x = 0;
			break;
		// da direita pra esquerda
		case 3:
			position.y = rand() % 620;
			position.x = 800;
			break;
	}
}