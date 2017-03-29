#pragma once
#include "Model.h"
#include "Camera.h"
enum KeyPressed
{
	W,
	A,
	S,
	D,
	SPACE
};
class Player
{
private:
	Model m_playerModel;
public:
	Camera m_camera;
	Player();
	~Player();
	void ProcessKeyboard(KeyPressed key, GLfloat deltaTime);
};

