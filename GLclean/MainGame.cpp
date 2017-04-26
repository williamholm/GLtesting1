#include "MainGame.h"

GLuint generateAttachmentTexture(GLboolean depth, GLboolean stencil);

const GLuint WIDTH = 800, HEIGHT = 600;

MainGame::MainGame():m_window(WIDTH, HEIGHT, "clean"), m_inputManager(m_window), m_camera(glm::vec3(0.0f, 0.0f, 3.0f)), m_deltaTime(0.0f),
					m_lastFrame(0.0f)
{
	initSystems();
}

MainGame::~MainGame()
{
}

void MainGame::initSystems()
{
	m_renderMaster.loadTerrain(&m_terrain);
}


void MainGame::gameLoop()
{
	m_lights.emplace_back(Cube(0.1, glm::vec3(0, 0, 0), m_shaders[ShaderType::LIGHTOBJ]));
	m_lightPositions.emplace_back(glm::vec3(0, 0, 0));
	Model person("ModelStuff/NanoSluit/C1.obj", glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));
	m_renderMaster.addModel(&person);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	while (!glfwWindowShouldClose(m_window.m_window))
	{
		displayInfo();
		m_inputManager.updateCallBacks();
		m_camera.processEvents(m_deltaTime);
		person.updatePosition(glm::vec3(m_camera.Position.x, m_terrain.getHeight(person.m_position.x, person.m_position.z), m_camera.Position.z - 3));

		m_projection = glm::perspective(m_camera.Zoom, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.01f, 100.0f);
		m_view = m_camera.GetViewMatrix(); 

		m_renderMaster.updateShaderInfo(m_view, m_projection, m_camera.Position, m_lightPositions);
		m_renderMaster.drawSequence();

		glfwSwapBuffers(m_window.m_window);
	}

	glfwTerminate();
	return;
}

void MainGame::displayInfo()
{
	GLfloat currentFrame = glfwGetTime();
	m_deltaTime = currentFrame - m_lastFrame;
	m_lastFrame = currentFrame;
	std::cout << "FPS: " << 1 / m_deltaTime << "\n";
}

GLuint generateAttachmentTexture(GLboolean depth, GLboolean stencil)
{
	// What enum to use?
	GLenum attachment_type;
	if (!depth && !stencil)
		attachment_type = GL_RGB;
	else if (depth && !stencil)
		attachment_type = GL_DEPTH_COMPONENT;
	else if (!depth && stencil)
		attachment_type = GL_STENCIL_INDEX;

	//Generate texture ID and load texture data 
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	if (!depth && !stencil)
		glTexImage2D(GL_TEXTURE_2D, 0, attachment_type, WIDTH, HEIGHT, 0, attachment_type, GL_UNSIGNED_BYTE, NULL);
	else // Using both a stencil and depth test, needs special format arguments
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, WIDTH, HEIGHT, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	return textureID;
}