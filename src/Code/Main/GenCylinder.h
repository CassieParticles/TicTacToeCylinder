#pragma once

struct CylinderSide
{
	const unsigned int id;
	const unsigned int EBO;
	const unsigned int posVbo;
	const unsigned int texVbo;
	const int vertices;
};

struct CylinderCaps
{
	const unsigned int id;
	const unsigned int EBO;
	const unsigned int posVbo;
	const int vertices;
};

struct Cylinder
{
	CylinderSide side;
	CylinderCaps caps;
};

Cylinder genSide(int sides);