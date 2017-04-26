#include "InputManager.h"

void m_keyCallBack(GLFWwindow* window, int key, int scancode, int action, int mode);
void m_mouseCallBack(GLFWwindow* window, double xpos, double ypos);
void m_scrollCallBack(GLFWwindow* window, double xoffset, double yoffset);
void m_mouseButtonCallBack(GLFWwindow* window, int key, int action, int mode);

static bool f_keys[1024];
static GLfloat f_lastX = 800 / 2.0;
static GLfloat f_lastY = 600 / 2.0;
static GLfloat f_offsetX;
static GLfloat f_offsetY;
static GLfloat f_scrollOfSet;
static bool f_firstMouse = true;

InputManager::InputManager(Window window)
{

	glfwSetKeyCallback(window.m_window, m_keyCallBack);
	glfwSetCursorPosCallback(window.m_window, m_mouseCallBack);
	glfwSetScrollCallback(window.m_window, m_scrollCallBack);
	glfwSetMouseButtonCallback(window.m_window, m_mouseButtonCallBack);


}
InputManager::~InputManager()
{
}


bool InputManager::isKeyDown(int keyCode)
{
	return f_keys[keyCode];
}

bool InputManager::isKeyPressed(int keyCode)
{
	if (f_keys[keyCode])
	{
		f_keys[keyCode] = false;
		return true;
	}
	else return f_keys[keyCode];
}
void InputManager::updateCallBacks()
{
	f_offsetX = 0;
	f_offsetY = 0;
	f_scrollOfSet = 0;
	glfwPollEvents();
}

glm::vec2 InputManager::getMouseCoords()
{
	return glm::vec2(f_lastX, f_lastY);
}

glm::vec2 InputManager::getMouseOffSet()
{
	return glm::vec2(f_offsetX, f_offsetY);
}

GLfloat InputManager::getMouseScrollOffSet()
{
	return f_scrollOfSet;
}

#pragma region callBacks

void m_keyCallBack(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			f_keys[key] = true;
		else if (action == GLFW_RELEASE)
			f_keys[key] = false;
	}

}

void m_mouseCallBack(GLFWwindow* window, double xpos, double ypos)
{
	if (f_firstMouse)
	{
		f_lastX = xpos;
		f_lastY = ypos;
		f_firstMouse = false;
	}

	f_offsetX = xpos - f_lastX;
	f_offsetY= f_lastY - ypos;  // Reversed since y-coordinates go from bottom to left

	f_lastX = xpos;
	f_lastY = ypos;

}


void m_scrollCallBack(GLFWwindow* window, double xoffset, double yoffset)
{
	f_scrollOfSet = yoffset;
}

void m_mouseButtonCallBack(GLFWwindow* window, int key, int action, int mode)
{
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			f_keys[key] = true;
		else if (action == GLFW_RELEASE)
			f_keys[key] = false;
	}

}

#pragma endregion
