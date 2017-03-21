#include "Window.h"
#include <iostream>
#include <cassert>

Window::Window()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
}


Window::~Window()
{
}

void Window::init(int width, int height, char windowName[])
{
	m_window = glfwCreateWindow(width, height, windowName, nullptr, nullptr);
	glfwMakeContextCurrent(m_window);

	glewExperimental = GL_TRUE; //allows more mordern opengl to be used
	if (glewInit() != GLEW_OK)
	{
		std::cout << "glew failed to init\n";
		exit(-1);
	}

	int fb_width, fb_height;
	glfwGetFramebufferSize(m_window, &fb_width, &fb_height); //width set to window width, height set to height width.
	glViewport(0, 0, fb_width, fb_height); //tells open gl where to render too.   0,0 sets the start location to lower left corner of window!
	glfwSetInputMode(this->m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glDepthFunc(GL_LESS);

}
