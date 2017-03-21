#define GLEW_STATIC
//#include <GL\glew.h>
//#include <GLFW\glfw3.h>
//
//#include <iostream>
//#include <string>
//
//
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
//
//#include "Shader.h"

#include "Cube.h"
#include "Camera.h"
#include "Window.h"


void keyCallBack(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouseCallBack(GLFWwindow* window, double xpos, double ypos);
void scrollCallBack(GLFWwindow* window, double xoffset, double yoffset);
void mouseButtonCallBack(GLFWwindow* window, int key, int action, int mode);
void do_movement();
void placeBlock();
void highLightStart(); ///CREATE GOOD HIGHLIGHT FUNCTIONS LATER!!!!!!!!!! -note- + 0.05 scaling on highlight looks good
void highLightEnd();   /// REFER BACK TO TUTORIAL!!!!
GLuint generateAttachmentTexture(GLboolean depth, GLboolean stencil);

const GLuint WIDTH = 800, HEIGHT = 600;

// Camera
Camera  camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;

bool    keys[1024];
bool	isShaderSet = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

bool firstMouse = true;
std::vector<Cube> cubes;
std::vector<Cube> lights;
std::vector<Shader> shaders;
std::vector<glm::vec3> lightPositions;

enum ShaderType
{
	LIGHTING,
	LIGHTOBJ,
	MODEL,
	SINGLECOLOUR,
	SCREEN_SHADER
};

int main()
{
	Window window;
	window.init(WIDTH, HEIGHT, "Graph\n");

	glfwSetKeyCallback(window.m_window, keyCallBack);
	glfwSetCursorPosCallback(window.m_window, mouseCallBack);
	glfwSetScrollCallback(window.m_window, scrollCallBack);
	glfwSetMouseButtonCallback(window.m_window, mouseButtonCallBack);


	shaders.emplace_back(Shader("vertShaderLight.txt", "fragShaderLight.txt"));
	shaders.emplace_back(Shader("vertObjShader.txt", "fragLightObjShader.txt"));
	shaders.emplace_back(Shader("vertModel.txt", "fragModel.txt"));
	shaders.emplace_back(Shader("vertexShader.txt", "singleColourFrag.txt"));
	shaders.emplace_back(Shader("vertSimple.txt", "fragSimple.txt"));

	// Set our texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glEnable(GL_DEPTH_TEST);//enables depth testing
							//	glEnable(GL_CULL_FACE);
							//	glCullFace(GL_BACK);
							//	glFrontFace(GL_CW);
							//	glEnable(GL_BLEND);
							//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glm::mat4 view;
	glm::mat4 projection;

	GLuint QuadVAO;
	GLuint QuadVBO;
	glGenBuffers(1, &QuadVBO);
	glGenVertexArrays(1, &QuadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, QuadVBO);
	glBindVertexArray(QuadVAO);
	GLfloat vertexPositions[]
	{
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		1.0f, -1.0f,  1.0f, 0.0f,
		1.0f,  1.0f,  1.0f, 1.0f
	};

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	while (!glfwWindowShouldClose(window.m_window))
	{

		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		std::cout << 1 / deltaTime << "\n";

		glfwPollEvents();
		do_movement();
		placeBlock();

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 


		view = camera.GetViewMatrix();


		glfwSwapBuffers(window.m_window);
	}

	glfwTerminate(); //cleans up all memory allocated to glfw
	return 0;
}

void do_movement()
{
	// Camera controls
	if (keys[GLFW_KEY_W])
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (keys[GLFW_KEY_S])
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A])
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (keys[GLFW_KEY_D])
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

void keyCallBack(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

void mouseCallBack(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos;  // Reversed since y-coordinates go from bottom to left

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}


void scrollCallBack(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

void mouseButtonCallBack(GLFWwindow* window, int key, int action, int mode)
{
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}

}

void placeBlock()
{

	if (keys[GLFW_MOUSE_BUTTON_1])
	{
		cubes.emplace_back(Cube(1, glm::vec3(camera.Position.x + 2 * camera.Front.x, camera.Position.y + 2 * camera.Front.y, camera.Position.z + 2 * camera.Front.z),
			shaders[ShaderType::LIGHTING], "container.jpg"));
		keys[GLFW_MOUSE_BUTTON_1] = false;
	}
	if (keys[GLFW_MOUSE_BUTTON_2])
	{
		lights.emplace_back(Cube(0.3, glm::vec3(camera.Position.x + 2 * camera.Front.x, camera.Position.y + 2 * camera.Front.y, camera.Position.z + 2 * camera.Front.z),
			shaders[ShaderType::LIGHTOBJ]));
		lightPositions.emplace_back(glm::vec3(camera.Position.x + 2 * camera.Front.x, camera.Position.y + 2 * camera.Front.y, camera.Position.z + 2 * camera.Front.z));
		keys[GLFW_MOUSE_BUTTON_2] = false;
	}
}
void highLightStart()
{
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilMask(0x00); // Disable writing to the stencil buffer
	glDisable(GL_DEPTH_TEST);

}

void highLightEnd()
{
	glStencilMask(0xFF);
	glEnable(GL_DEPTH_TEST);
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