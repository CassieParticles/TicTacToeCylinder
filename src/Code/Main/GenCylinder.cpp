#include<glad/glad.h>

#include <glm.hpp>
#include <gtc/constants.hpp>
#include <iostream>
#include "GenCylinder.h"

Cylinder genSide(int sides)
{
	float* vertexPosition{ new float[sides * 6 + 6] {} };
	int* sideIndices{ new int[sides * 6]{} };
	int* capIndices{ new int[(sides - 2) * 6] };
	float* textureCoords{ new float[sides * 4 + 4] };

	constexpr double fullRotation = glm::pi<double>() * 2;
	int index = 0;

	for (double r = 0; r <= 1; r += 1.0 / sides, index += 1)	//Construct vertice positions and textureCoords array
	{
		vertexPosition[index * 3 + 0] = cos(r * fullRotation);
		vertexPosition[index * 3 + 1] = 1.5f;
		vertexPosition[index * 3 + 2] = sin(r * fullRotation);

		textureCoords[index * 2 + 0] = r;
		textureCoords[index * 2 + 1] = 0;

		vertexPosition[(index + sides + 1) * 3 + 0] = cos(r * fullRotation);
		vertexPosition[(index + sides + 1) * 3 + 1] = -1.5f;
		vertexPosition[(index + sides + 1) * 3 + 2] = sin(r * fullRotation);

		textureCoords[(index + sides + 1) * 2 + 0] = r;
		textureCoords[(index + sides + 1) * 2 + 1] = 1;
	}

	for (index = 0; index < sides; index++)	//Construct the indices
	{
		sideIndices[index * 6 + 0] = index;
		sideIndices[index * 6 + 1] = index + 1;
		sideIndices[index * 6 + 2] = index + sides + 2;

		sideIndices[index * 6 + 3] = index;
		sideIndices[index * 6 + 4] = index + sides + 2;
		sideIndices[index * 6 + 5] = index + sides + 1;
	}


	for (int i = 0; i < sides - 2; i++)
	{
		capIndices[(i * 3) + 0] = 0;
		capIndices[(i * 3) + 1] = i+2;
		capIndices[(i * 3) + 2] = i+1;
	}

	for (int i = 0; i < sides - 2; i++)
	{
		capIndices[(i + sides - 2) * 3 + 0] = sides + 1;
		capIndices[(i + sides - 2) * 3 + 1] = sides + i + 2;
		capIndices[(i + sides - 2) * 3 + 2] = sides + i + 3;
	}


	unsigned int sideVaoId;		
	unsigned int capVaoId;
	unsigned int vertexPosVboId;
	unsigned int vertexTexVboId;
	unsigned int sideEboId;
	unsigned int capEboId;

	glGenVertexArrays(1, &sideVaoId);	//Generate cylidner side vertex info
	glGenBuffers(1, &vertexPosVboId);	//Vertex info used in sides and cap
	glGenBuffers(1, &vertexTexVboId);
	glGenBuffers(1, &sideEboId);

	glBindVertexArray(sideVaoId);
	glBindBuffer(GL_ARRAY_BUFFER, vertexPosVboId);
	glBufferData(GL_ARRAY_BUFFER, (sides * 6 + 6) * sizeof(float), vertexPosition, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vertexTexVboId);
	glBufferData(GL_ARRAY_BUFFER, (sides * 4 + 4) * sizeof(float), textureCoords, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sideEboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sides * 6 * sizeof(int), sideIndices, GL_STATIC_DRAW);
	glBindVertexArray(0);

	glGenVertexArrays(1, &capVaoId);
	glGenBuffers(1, &capEboId);

	glBindVertexArray(capVaoId);
	glBindBuffer(GL_ARRAY_BUFFER, vertexPosVboId);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, capEboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (sides - 2) * 6 * sizeof(int), capIndices, GL_STATIC_DRAW);

	CylinderSide cylinderSide = { sideVaoId,sideEboId,vertexPosVboId,vertexTexVboId,sides * 6 };
	CylinderCaps cylinderCap = { capVaoId,capEboId,vertexPosVboId,(sides - 2) * 6 };

	Cylinder cylinder{ cylinderSide,cylinderCap };

	delete[] sideIndices;
	delete[] textureCoords;
	delete[] vertexPosition;
	delete[] capIndices;

	return cylinder;
}