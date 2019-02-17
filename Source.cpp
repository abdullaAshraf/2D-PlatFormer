#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "Player.h"
#include "Block.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

Player player;

int main()
{
	// glfw: initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw window creation
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// build and compile our shader program
	Shader ourShader("shaders/basicShader.vs", "shaders/basicShader.fs");

	player.makePlayer();

	Block blocks[] = {
		Block(-1, -0.25f, -0.5f, -0.5f),
		Block(-1, 0.5f, -0.25f, 0.25f),
		Block(0.75f, 0.5f, 1.5f, 0.25f),
		Block(-2, -1, 2, -1.25f),
		Block(-2, 2, -1.6f, -2),
		Block(1.6, 2, 2, -2),
	};

	ourShader.use();
	glm::mat4 projection = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
	ourShader.setMat4("projection", projection);
	ourShader.setMat4("view", view);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		processInput(window);

		// render
		glClearColor(21.0 / 255, 54.0 / 255, 65.0 / 255, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		player.canJump = false;
		for (int i = 0; i < (sizeof(blocks) / sizeof(*blocks)); i++) {
			blocks[i].handleCollision(&player);
			blocks[i].drawBlock(ourShader);
		}
		
		player.updatePosition(deltaTime);
		player.drawPlayer(ourShader);

		// glfw: swap buffers and poll IO events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	// glfw: terminate, clearing all previously allocated GLFW resources.
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		player.forceX = -1 * player.moveSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		player.forceX = player.moveSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		if (player.canJump)
			player.forceY = player.jumpForce;
	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}