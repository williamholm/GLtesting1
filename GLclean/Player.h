#pragma once
#include "Model.h"
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
	Player();
	~Player();
	void ProcessKeyboard(KeyPressed key, GLfloat deltaTime);
};

