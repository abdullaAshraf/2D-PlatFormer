#pragma once
#include <glad/glad.h>

#include <string>
#include <algorithm>

#include "shader.h"
#include "Player.h"
class Block
{
public:
	unsigned int VAO;
	float  x1, y1, x2, y2 , slideFactor;

	void drawBlock(Shader shader);

	void handleCollision(Player* player);

	Block(float x1, float y1 , float x2 , float y2);
	~Block();

private:
	unsigned int drawRect(float x1, float y1, float x2, float y2,float color[3]) {
		float vertices[] = {
			x1 , y1 , 0.0f , color[0] , color[1] , color[2],
			x2 , y1 , 0.0f , color[0] , color[1] , color[2],
			x2 , y2 , 0.0f , color[0] , color[1] , color[2],
			x1 , y2 , 0.0f , color[0] , color[1] , color[2]
		};
		int indices[] = {
			0,1,2,
			0,2,3
		};
		unsigned int VBO, VAO, EBO;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
		// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// color coord attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindVertexArray(0);
		return VAO;
	}
};

