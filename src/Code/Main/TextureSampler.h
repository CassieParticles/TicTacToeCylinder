#pragma once

#include <glm.hpp>

namespace TextureSampler{
	void init();

	glm::vec4 sampleTexture(double x, double y, unsigned int texture);
}

