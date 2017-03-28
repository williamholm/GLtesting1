#include "Player.h"



Player::Player()
{
}


Player::~Player()
{
}

void Player::ProcessKeyboard(KeyPressed direction, GLfloat deltaTime)
{
//	GLfloat velocity = this->MovementSpeed * deltaTime;
//	if (direction == FORWARD)
//		this->Position += this->Front * velocity;
//
//	//			this->Position += glm::vec3(Front.x, 0, Front.z) * velocity;
//	if (direction == BACKWARD)
//		this->Position -= this->Front * velocity;
//
//	//			this->Position -= glm::vec3(Front.x, 0, Front.z) * velocity; //this for no moving up / down but being able to look up / down
//	if (direction == LEFT)
//		this->Position -= this->Right * velocity;
//	if (direction == RIGHT)
//		this->Position += this->Right * velocity;
}