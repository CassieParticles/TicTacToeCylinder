#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <iostream>
#include <Windows.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "../Utils/ProgramShader.h"
#include "../Utils/Timer.h"
#include "GenCylinder.h"
#include "BoardRender.h"
#include "TicTacToe.h"
#include "TextureSampler.h"


GLFWwindow* window;
glm::mat4 projectionMatrix;
glm::mat4 rotationMatrix;

unsigned int screenVao{};
unsigned int screenBuffers[3];	//0-position,1-textureCoord,2-indexEbo

int screenWidth{800};
int screenHeight{800};

float vertices[8] = {
-1.0f, 1.0f,
 1.0f, 1.0f,
 1.0f,-1.0f,
-1.0f,-1.0f
};

float textureCoords[8] = {
	0.0f,1.0f,
	1.0f,1.0f,
	1.0f,0.0f,
	0.0f,0.0f
};

int indices[6] = {
	0,2,1,
	0,3,2
};

ProgramShader::Program sideProgram{};
ProgramShader::Program capProgram{};

ProgramShader::Program screenProgram{};

unsigned int boardFrameBuffer;
unsigned int boardTexture;

unsigned int sceneFrameBuffer;
unsigned int sceneFrameTextures[2];		//0-colour, 1-texture co-ords (for input detection)
unsigned int depthRenderBuffer;

float dist = 3;

bool initWindow()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	window = glfwCreateWindow(screenWidth, screenHeight, "Tic tac toe", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create a window" << std::endl;
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD";
		glfwTerminate();
		return false;
	}
	

	glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height)->void {glViewport(0, 0, width, height); screenWidth = width; screenHeight = height; });

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return true;
}

void render(Cylinder cylinder)
{
	glClearColor(0.2f, 0.2f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_BLEND);

	glDisable(GL_DEPTH_TEST);

	glBindFramebuffer(GL_FRAMEBUFFER, boardFrameBuffer);

	glViewport(0, 0, 1024, 1024);

	boardRender::renderBoard(getBoard());

	glBindFramebuffer(GL_FRAMEBUFFER, sceneFrameBuffer);
	glViewport(0, 0, 1024, 1024);
	GLuint drawBuffers[2] = { GL_COLOR_ATTACHMENT0,GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, drawBuffers);

	glEnable(GL_DEPTH_TEST);

	glClearColor(0.2f, 0.2f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDisable(GL_BLEND);

	//Rendering cylinder
	glClearColor(0.2f, 0.2f, 0.4f, 0.0f);
	sideProgram.use();
	sideProgram.setMat4("projectionMatrix", projectionMatrix);
	sideProgram.setMat4("rotationMatrix", rotationMatrix);
	sideProgram.setFloat("dist", dist);
	sideProgram.setInt("ourTexture", 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, boardTexture);
	glBindVertexArray(cylinder.side.id);
	glDrawElements(GL_TRIANGLES, cylinder.side.vertices, GL_UNSIGNED_INT, 0);

	capProgram.use();
	capProgram.setMat4("projectionMatrix", projectionMatrix);
	capProgram.setMat4("rotationMatrix", rotationMatrix);
	capProgram.setFloat("dist", dist);
	capProgram.setVec3("colour", glm::vec3(0.4f, 0.0f, 0.7f));
	glBindVertexArray(cylinder.caps.id);
	glDrawElements(GL_TRIANGLES, cylinder.caps.vertices, GL_UNSIGNED_INT, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);	//Rendering the screen
	glDisable(GL_DEPTH_TEST);
	glViewport(0, 0, screenWidth, screenHeight);

	glClearColor(0.2f, 0.2f, 0.4f, 1.0f);

	screenProgram.use();
	screenProgram.setInt("tex", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, sceneFrameTextures[0]);
	glBindVertexArray(screenVao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glfwSwapBuffers(window);	//Check and call events
	glfwPollEvents();

}

void input()
{
	constexpr float rotSpeed = 3.0;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		rotationMatrix=glm::rotate(glm::mat4(1.0), rotSpeed*static_cast<float>(Timer::getDeltaTime()), glm::vec3(0, 1, 0)) * rotationMatrix;
	}if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		rotationMatrix=glm::rotate(glm::mat4(1.0), -rotSpeed*static_cast<float>(Timer::getDeltaTime()), glm::vec3(0, 1, 0)) * rotationMatrix;
	}if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		rotationMatrix=glm::rotate(glm::mat4(1.0), rotSpeed*static_cast<float>(Timer::getDeltaTime()), glm::vec3(1, 0, 0)) * rotationMatrix;
	}if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		rotationMatrix=glm::rotate(glm::mat4(1.0), -rotSpeed*static_cast<float>(Timer::getDeltaTime()), glm::vec3(1, 0, 0)) * rotationMatrix;
	}if (glfwGetKey(window, GLFW_KEY_MINUS)==GLFW_PRESS)
	{
		dist += Timer::getDeltaTime();
		if (dist > 20.0f)
		{
			dist = 20.0f;
		}
	}if (glfwGetKey(window, GLFW_KEY_EQUAL)==GLFW_PRESS)
	{
		dist -= Timer::getDeltaTime();
		if (dist < 2.0f)
		{
			dist = 2.0f;
		}
	}
}




int main()
{
	bool initSuccess=initWindow();
	if (!initSuccess)
	{
		return -1;
	}

	boardRender::init();

	sideProgram.init("src/Shaders/Cylinder/sideVertex.glsl", "src/Shaders/Cylinder/sideFragment.glsl");
	capProgram.init("src/Shaders/Cylinder/capVertex.glsl", "src/Shaders/Cylinder/capFragment.glsl");

	screenProgram.init("src/Shaders/Screen/vertex.glsl", "src/Shaders/Screen/fragment.glsl");

	Timer::init(60.0);

	TextureSampler::init();



	Cylinder cylinder = genSide(90);

	glGenVertexArrays(1, &screenVao);
	glGenBuffers(3, screenBuffers);
	glBindVertexArray(screenVao);

	glBindBuffer(GL_ARRAY_BUFFER, screenBuffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, screenBuffers[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(textureCoords), textureCoords, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, screenBuffers[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	projectionMatrix = glm::perspective(glm::radians(90.0f),1.0f, 0.1f, 100.0f);
	rotationMatrix = glm::mat4(1.0);

	glGenFramebuffers(1, &boardFrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, boardFrameBuffer);

	glGenTextures(1, &boardTexture);
	glBindTexture(GL_TEXTURE_2D, boardTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024, 1024, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, boardTexture, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Board framebuffer failed to generate";
		return -1;
	}
	glBindFramebuffer(GL_FRAMEBUFFER,0);

	
	glGenFramebuffers(1, &sceneFrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, sceneFrameBuffer);

	glGenTextures(2, sceneFrameTextures);	//Generate both at once

	glBindTexture(GL_TEXTURE_2D, sceneFrameTextures[0]);	//Colour
	glTexImage2D(GL_TEXTURE_2D,0, GL_RGB, 1024, 1024, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, sceneFrameTextures[0], 0);

	glBindTexture(GL_TEXTURE_2D, sceneFrameTextures[1]);	//Text-coords (z: 1-part of board,0-not part of board)
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024, 1024, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, sceneFrameTextures[1], 0);

	glGenRenderbuffers(1, &depthRenderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1024, 1024);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthRenderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Scene framebuffer failed to generate";
		return -1;
	}

	glBindFramebuffer(GL_FRAMEBUFFER,0);

	int playerTurn = 1;

	bool mouseUp = true;
	bool gamePlaying = true;

	while (!glfwWindowShouldClose(window))
	{
		if (Timer::Update()) {
			input();
			render(cylinder);
		}
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS&&mouseUp&&gamePlaying)
		{
			double xPos{}, yPos{};
			int xCol=-1, yCol=-1;

			glfwGetCursorPos(window, &xPos, &yPos);

			glm::vec4 position= TextureSampler::sampleTexture(xPos / screenWidth, yPos / screenHeight, sceneFrameTextures[1]);
			if (position.z == 1)
			{
				xCol = floor(position.x * 3);
				yCol = floor((1-position.y) * 3);

				if (xCol != -1 && yCol != -1)
				{
					int winner = makeMove(yCol % 3, xCol % 3, playerTurn);
					if (winner == 2)
					{
						std::cout << "That space is taken\n";
						winner = 0;
					}
					else if (winner == 1)
					{
						std::cout << "0 has won!\n";
						gamePlaying = false;
					}
					else if (winner == -1)
					{
						std::cout << "X has won!\n";
						gamePlaying = false;
					}
					else
					{
						playerTurn *= -1;
					}
				}
			}
			mouseUp = false;
		}


		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_RELEASE)
		{
			mouseUp = true;
		}
	}
	glDeleteFramebuffers(1,&boardFrameBuffer);
	glDeleteFramebuffers(1, &sceneFrameBuffer);
	return 0;
}