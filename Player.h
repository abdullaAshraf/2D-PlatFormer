#pragma once
#include <glad/glad.h>

#include <string>
#include <algorithm>

#include "shader.h"

const double degToRad = 3.14159 / 180;
const int resolution = 48;

class Player
{
public:
	unsigned int VAO;
	float  posX, posY, forceX, forceY, moveSpeed, gravity, drag, jumpForce, rad;
	bool canJump;

	Player();

	~Player();

	void drawPlayer(Shader ourShader);

	void updatePosition(float deltaTime);

	void makePlayer();
private:
	unsigned int drawCircle(float rad, float color[3]) {
		float vertices[(resolution + 1) * 6];
		int indices[resolution * 3];
		float theta = 360.0 / resolution;
		//initlize center point
		vertices[0] = vertices[1] = vertices[2] = 0.0f;
		vertices[3] = color[0];
		vertices[4] = color[1];
		vertices[5] = color[2];
		//add other points to array
		for (int i = 1; i <= resolution; i++) {
			vertices[i * 6] = rad * cos(theta * (i - 1) * degToRad);
			vertices[i * 6 + 1] = rad * sin(theta * (i - 1) * degToRad);
			vertices[i * 6 + 2] = 0.0f;
			vertices[i * 6 + 3] = color[0];
			vertices[i * 6 + 4] = color[1];
			vertices[i * 6 + 5] = color[2];
		}

		for (int i = 0; i < resolution; i++) {
			indices[i * 3] = 0;
			indices[i * 3 + 1] = i + 1;
			indices[i * 3 + 2] = (i + 2 > resolution ? 1 : i + 2);
		}

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
