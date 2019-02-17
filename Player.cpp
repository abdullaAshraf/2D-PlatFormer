#include <glad/glad.h>
#include <glm/glm.hpp>

#include <glm/gtc/matrix_transform.hpp>

#include "Player.h"
#include "shader.h"

Player::Player()
{
	posX = posY = forceX = forceY = 0.0f;
	moveSpeed = 1;
	gravity = 3;
	drag = 10;
	jumpForce = 2;
	rad = 0.2f;
	canJump = false;
}

void Player::makePlayer() {
	float color[] = { 128.0 / 255,96.0 / 255,204.0 / 255 };
	VAO = Player::drawCircle(rad, color);
}

void Player::drawPlayer(Shader ourShader) {
	ourShader.use();
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(posX, posY, 0.0f));
	ourShader.setMat4("model", model);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, resolution * 3, GL_UNSIGNED_INT, 0);
}

void  Player::updatePosition(float deltaTime) {
	posX += forceX * deltaTime;
	posY += forceY * deltaTime;

	if (forceX > 0)
		forceX = std::max(0.0f, forceX - drag * deltaTime);
	else
		forceX = std::min(0.0f, forceX + drag * deltaTime);

	forceY = forceY - gravity * deltaTime;
}


Player::~Player()
{
}
