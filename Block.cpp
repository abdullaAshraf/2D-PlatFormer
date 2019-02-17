#include <glad/glad.h>
#include <glm/glm.hpp>

#include <glm/gtc/matrix_transform.hpp>

#include "Block.h"
#include "Player.h"
#include "shader.h"

Block::Block(float _x1, float _y1, float _x2, float _y2)
{
	slideFactor = 3;
	x1 = _x1;
	y1 = _y1;
	x2 = _x2;
	y2 = _y2;
	float color[] = { 71.0 / 255,153.0 / 255,183.0 / 255 };
	VAO = Block::drawRect(x1, y1, x2, y2, color);
}

void Block::drawBlock(Shader shader) {
	shader.use();
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	shader.setMat4("model", model);
	//TODO edit view matrix for camera movement
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Block::handleCollision(Player* player) {
	// Get center point circle
	glm::vec2 center(player->posX, player->posY);
	// Calculate AABB info (center, half-extents)
	glm::vec2 aabb_half_extents((x2 - x1) / 2, (y1 - y2) / 2);
	glm::vec2 aabb_center(
		x1 + aabb_half_extents.x,
		y2 + aabb_half_extents.y
	);
	// Get difference vector between both centers
	glm::vec2 difference = center - aabb_center;
	glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
	// Add clamped value to AABB_center and we get the value of box closest to circle
	glm::vec2 closest = aabb_center + clamped;
	// Retrieve vector between center circle and closest point AABB and check if length <= radius
	difference = center - closest;
	if (glm::length(difference) < player->rad) {
		if (closest.y == y1) { //from above
			player->forceY = std::max(player->forceY, 0.0f);
			player->canJump = true;
			if (player->posX > x2)
				player->forceX += slideFactor * (player->posX - x2);
			else if (player->posX < x1)
				player->forceX += slideFactor * (player->posX - x1);
		}
		else if (closest.y == y2) { //from below
			player->forceY = std::min(player->forceY, 0.0f);
		}
		else if (closest.x == x1) { //from left
			player->forceX = std::max(player->forceX, 0.0f);
		}
		else if (closest.x == x2) { //from right
			player->forceX = std::min(player->forceX, 0.0f);
		}

		float cosA = difference.y / glm::length(difference);
		float sinA = difference.x / glm::length(difference);
		player->posX = closest.x + player->rad * sinA;
		player->posY = closest.y + player->rad * cosA;
	}
	return;
	if (player->posX + player->rad > x1 && player->posX - player->rad < x2) { //vertical collision
		if (abs(player->posY - y1) < player->rad) { //from above
			player->forceY = std::max(player->forceY, 0.0f);
			player->posY = y1 + player->rad;
		}
		else if (abs(player->posY - y2) < player->rad) { //from below
			player->forceY = std::min(player->forceY, 0.0f);
			player->posY = y2 - player->rad;
		}
	}
	if (player->posY + player->rad > y2 && player->posY - player->rad < y1) { //horizontal collision
		if (abs(player->posX - x1) < player->rad) {
			player->forceX = std::max(player->forceX, 0.0f);
			player->posX = x1 - player->rad;
		}
		else if (abs(player->posX - x2) < player->rad) {
			player->forceX = std::min(player->forceX, 0.0f);
			player->posX = x2 + player->rad;
		}
	}
}

Block::~Block()
{
}
