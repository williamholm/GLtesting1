#pragma once
#include <GL\glew.h>
#include <GLFW\glfw3.h>

class Window
{
private:

public:

	void init(int width, int height, char windowName[]);
	GLFWwindow* m_window;

	Window();
	~Window();
};

