#include "Meteor.h"

void Meteor::update()
{	
	// saiu da tela, respawna outro, conta ponto
	if (position.y < 0 || position.y > 620 || position.x < 0 || position.x > 800)
	{	
		// gera aleatoriamente um modo de disparo (direções horizontais e verticais)
		m = (rand() % 4);
		respawn(m);
	}

	switch (m) {
		case 0:
			position.y -= vel;
			break;
		case 1:
			position.y += vel;
			break;
		case 2:
			position.x += vel;
			break;
		case 3:
			position.x -= vel;
			break;
	}

	//Matriz de modelo: transformações no objeto
	glm::mat4 model = glm::mat4(1); //matriz identidade

	model = glm::translate(model, position);
	model = glm::rotate(model, angle, axis);
	model = glm::scale(model, scale);

	//Enviando a matriz de modelo para o shader
	shader->setMat4("model", glm::value_ptr(model));

	//Enviando para o shader o deslocamento das coords de textura
	//de acordo com os indices de animação e frame
	iFrame = (iFrame + 1) % nFrames;
	shader->setVec2("offsets", iFrame * dx, iAnimation * dy);

}

void Meteor::respawn(int m) {
	switch(m) {
		// de cima pra baixo
		case 0:
			std::cout << 0 << std::endl;
			// (padrão)
			position.y = 620;
			position.x = rand() % 800;
			break;
		// de baixo pra cima
		case 1:
			std::cout << 1 << std::endl;
			position.y = 0;
			position.x = rand() % 800;
			break;
		// da esquerda pra direita
		case 2:
			std::cout << 2 << std::endl;
			position.y = rand() % 620;
			position.x = 0;
			break;
		// da direita pra esquerda
		case 3:
			std::cout << 3 << std::endl;
			position.y = rand() % 620;
			position.x = 800;
			break;
	}
}