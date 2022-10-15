#include "Timer.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Timer
{
	double lastTime = {};
	double currentTime = {};
	double updateTime = {};

	double deltaTime = {};

	void init(double UPS)
	{
		updateTime = 1 / UPS;
		lastTime = glfwGetTime();
		currentTime = lastTime;
		deltaTime = 0;
	}

	bool Update()
	{
		currentTime = glfwGetTime();
		bool update = currentTime - lastTime > updateTime;
		if (update)
		{
			deltaTime = currentTime - lastTime;
			lastTime = currentTime;
		}
		return update;
	}

	double getDeltaTime()
	{
		return deltaTime;
	}
}