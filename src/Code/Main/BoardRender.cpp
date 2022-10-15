#include <glad/glad.h>
#include <glm.hpp>
#include <iostream>

#include <stb_image.h>

#include "BoardRender.h"
#include "../Utils/ProgramShader.h"

namespace boardRender
{
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

	ProgramShader::Program boardProgram{};
	ProgramShader::Program pieceProgram{};

	unsigned int vaoId;
	unsigned int positionVboId;
	unsigned int texCoordVboId;
	unsigned int indexEboId;

	unsigned int boardTexture;
	unsigned int noughtTexture;
	unsigned int crossTexture;

	unsigned int genTexture(const char* filepath)
	{
		int width, height, channels;
		unsigned int tex{};

		unsigned char* data = stbi_load(filepath, &width, &height, &channels, 0);
		if (data)
		{
			glGenTextures(1, &tex);
			glBindTexture(GL_TEXTURE_2D, tex);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Error: texture " << filepath << " not loaded properly\n";
			tex = 0;
		}
		return tex;
	}

	void init()
	{

		boardProgram.init("src/Shaders/Screen/vertex.glsl", "src/Shaders/Screen/fragment.glsl");
		pieceProgram.init("src/Shaders/Board/pieceVertex.glsl","src/Shaders/Board/pieceFragment.glsl");

		glGenVertexArrays(1, &vaoId);		//Buffers
		glBindVertexArray(vaoId);

		glGenBuffers(1, &positionVboId);
		glBindBuffer(GL_ARRAY_BUFFER, positionVboId);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glGenBuffers(1, &texCoordVboId);
		glBindBuffer(GL_ARRAY_BUFFER, texCoordVboId);
		glBufferData(GL_ARRAY_BUFFER, sizeof(textureCoords), textureCoords, GL_STATIC_DRAW);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);

		glGenBuffers(1, &indexEboId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexEboId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		boardTexture = genTexture("src/Images/Board.png");
		noughtTexture = genTexture("src/Images/Nought.png");
		crossTexture = genTexture("src/Images/Cross.png");
	}

	void renderBoard(int* board)
	{
		boardProgram.use();			//Render board
		boardProgram.setInt("tex", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, boardTexture);
		glBindVertexArray(vaoId);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glm::vec2 piecePosition = {0.165f,0.172f};
		glm::vec2 pieceSize = { 0.318f,0.313f };

		pieceProgram.use();
		pieceProgram.setVec2("scale", pieceSize);
		pieceProgram.setInt("tex", 0);

		for (int x = 0; x < 3; x++)
		{
			for (int y = 0; y < 3; y++) {
				int index = x * 3 + y;
				if (!board[index])
				{
					piecePosition += glm::vec2{0.334f,0};
					continue;
				}
				pieceProgram.setVec2("translate", piecePosition * glm::vec2(2.0f,-2.0f) - glm::vec2{1.0f,-1.0f});
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, board[index] == 1 ? noughtTexture : crossTexture);
				glBindVertexArray(vaoId);
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
				piecePosition += glm::vec2{ 0.334f,0 };
			}
			piecePosition.x = 0.165f;
			piecePosition.y += 0.330;
		}
	}

}

