#include <glad/glad.h>
#include <iostream>
#include <glm.hpp>

#include "../Utils/ProgramShader.h"
#include "TextureSampler.h"

namespace TextureSampler {
	unsigned int frameBuffer;
	unsigned int texture;

	unsigned int vao;
	unsigned int vbos[3];	//0-position,1-textCoords,2-ebo

	ProgramShader::Program program;

	float vertices[8] = {
	-1.0f, 1.0f,
	 1.0f, 1.0f,
	 1.0f,-1.0f,
	-1.0f,-1.0f
	};

	float texCoords[8] = {
		0.0f,1.0f,
		1.0f,1.0f,
		1.0f,0.0f,
		0.0f,0.0f
	};

	int indices[6] = {
		0,2,1,
		0,3,2
	};


	void init()
	{
		std::cout << glGetError()<<'\n';
		glGenFramebuffers(1, &frameBuffer);	//Setup framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1024, 1024, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cout << "Board framebuffer failed to generate";
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glGenBuffers(3, vbos);

		glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_STATIC_DRAW);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos[2]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		glBindVertexArray(0);

		program = ProgramShader::Program{};

		program.init("src/Shaders/Screen/vertex.glsl","src/Shaders/Screen/fragment.glsl");

		std::cout << glGetError()<<'\n';

	}

	glm::vec4 sampleTexture(double x, double y, unsigned int texture)
	{
		int sampleX = floor(x * 1024);
		int sampleY = floor((1 - y) * 1024);

		glClearColor(0, 1, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT);

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);

		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
		glViewport(0, 0, 1024, 1024);

		GLuint drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, drawBuffers);

		program.use();
		program.setInt("tex", 0);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindTexture(GL_TEXTURE_2D, texture);

		float data[4]{};

		glReadPixels(sampleX, sampleY, 1, 1, GL_RGBA, GL_FLOAT, data);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glm::vec4 value = glm::vec4(data[0], data[1], data[2], data[3]);

		return value;
	}
}