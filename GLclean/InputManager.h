#pragma once
#include <glm\glm.hpp>
#include "Window.h"
#include "Camera.h"

class InputManager
{
private:
	glm::vec2 m_mouseCoords;

public:
	//used for continuous things i.e movement;
static	bool isKeyDown(int keyCode);
	//used for single time actions i.e jumping;
static	bool isKeyPressed(int keyCode);

static	glm::vec2 getMouseCoords();
static	glm::vec2 getMouseOffSet();
static	GLfloat getMouseScrollOffSet();
static	void updateCallBacks();

	InputManager(Window window);
	~InputManager();
};



