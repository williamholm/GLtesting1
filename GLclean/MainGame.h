#pragma once
#define GLEW_STATIC
#include "Cube.h"
#include "Camera.h"
#include "Window.h"
#include "MasterRenderer.h"
#include "InputManager.h"


class MainGame
{
public:
	Window m_window;
	Camera m_camera;
	

	std::vector<Shader> m_shaders;
	std::vector<Cube> m_cubes;
	std::vector<Cube> m_lights;
	std::vector<glm::vec3> m_lightPositions;


	glm::mat4x4 m_projection;
	glm::mat4x4 m_view;

	//frame buffer for drawing is single quad
	GLuint m_textureColourBuffer;
	GLuint m_quadVAO;
	GLuint m_FBDrawAsQuad;

	GLfloat m_deltaTime;
	GLfloat m_lastFrame;

	Terrain m_terrain;
	MasterRenderer m_renderMaster;
	InputManager m_inputManager;

private:
	//make to display fps,coords,no of models... ect
	void displayInfo();
public:
	void gameLoop();
	void initSystems();
	MainGame();
	~MainGame();
};

