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

#include "Model.h"

void keyCallBack(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouseCallBack(GLFWwindow* window, double xpos, double ypos);
void scrollCallBack(GLFWwindow* window, double xoffset, double yoffset);
void mouseButtonCallBack(GLFWwindow* window, int key, int action, int mode);
void do_movement();
void placeBlock();
void highLightStart(); ///CREATE GOOD HIGHLIGHT FUNCTIONS LATER!!!!!!!!!! -note- + 0.05 scaling on highlight looks good
void highLightEnd();   /// REFER BACK TO TUTORIAL!!!!gerge
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
	SCREEN_SHADER,
	MODEL_C
};

int main()
{
	Window window;
	window.init(WIDTH, HEIGHT, "CLEAN\n");

	glfwSetKeyCallback(window.m_window, keyCallBack);
	glfwSetCursorPosCallback(window.m_window, mouseCallBack);
	glfwSetScrollCallback(window.m_window, scrollCallBack);
	glfwSetMouseButtonCallback(window.m_window, mouseButtonCallBack);

	shaders.emplace_back(Shader("vertShaderLight.txt", "fragShaderLight.txt"));
	shaders.emplace_back(Shader("vertObjShader.txt", "fragLightObjShader.txt"));
	shaders.emplace_back(Shader("vertModel.txt", "fragModel.txt"));
	shaders.emplace_back(Shader("vertexShader.txt", "singleColourFrag.txt"));
	shaders.emplace_back(Shader("vertSimple.txt", "fragSimple.txt"));
	shaders.emplace_back(Shader("vertNoTextureModel.txt", "fragNoTextureModel.txt"));


	
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


	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < 2; ++j)
		{
			cubes.emplace_back(Cube(1, glm::vec3(i,-3, j), shaders[ShaderType::LIGHTING], "container.jpg"));
		}
	}

	cubes.emplace_back(Cube(1, glm::vec3(3, -2, 3), shaders[ShaderType::LIGHTING], "container.jpg"));
	cubes.emplace_back(Cube(1, glm::vec3(5, -2, 6), shaders[ShaderType::LIGHTING], "blending_transparent_window.png"));

	lights.emplace_back(Cube(0.3, glm::vec3(6, 1, 6), shaders[ShaderType::LIGHTOBJ]));
	lightPositions.emplace_back(glm::vec3(6, 1, 6));//31fps

	Model ourModel("ModelStuff/NanoSluit/simple_sword2.obj");//note nanosuit is ~~13k vertices
	Model g = ourModel;

	glm::mat4 view;
	glm::mat4 projection;


	GLfloat vertexPositions[]
	{
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		1.0f, -1.0f,  1.0f, 0.0f,
		1.0f,  1.0f,  1.0f, 1.0f
	};
	GLuint quadVAO, quadVBO;
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), &vertexPositions, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
	glBindVertexArray(0);


	// Framebuffers
	GLuint framebuffer;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	// Create a color attachment texture
	GLuint textureColorbuffer = generateAttachmentTexture(false, false);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
	// Create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
	GLuint rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600); // Use a single renderbuffer object for both a depth AND stencil buffer.
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // Now actually attach it
																								  // Now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	while (!glfwWindowShouldClose(window.m_window))
	{	
		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
	//	std::cout << 1 / deltaTime << "\n";
	
		glfwPollEvents();
		do_movement();
		placeBlock();
		
	
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glClearColor(0.1f, 0.3f, 0.7f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // We're not using stencil buffer now
	
		glEnable(GL_DEPTH_TEST);
		
		view = camera.GetViewMatrix();

	//	cubes[1].usingShader();
	//	for (int i = 0; i < cubes.size(); i++)
	//	{
	//		cubes[i].draw(view, camera.Zoom, camera.Position, lightPositions);	
	//	}
	
		lights[0].usingShader();
		for (int i = 0; i < lights.size() ; i++)
		{
			lights[i].draw(view, camera.Zoom);
		}
	
		shaders[ShaderType::MODEL_C].Use();
	
		projection = glm::perspective(camera.Zoom, (GLfloat)WIDTH/(GLfloat)HEIGHT, 0.01f, 100.0f);
	
		
		glUniformMatrix4fv(glGetUniformLocation(shaders[ShaderType::MODEL_C].Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(shaders[ShaderType::MODEL_C].Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		/// move this to the draw function in mesh?
		// Draw the loaded model 
		glm::mat4 model;
		model = glm::translate(model, glm::vec3(1.0f, -2.5f, 1.0f)); // Translate it down a bit so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// It's a bit too big for our scene, so scale it down
		glUniformMatrix4fv(glGetUniformLocation(shaders[ShaderType::MODEL_C].Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		
		
	
		g.Draw(shaders[ShaderType::MODEL_C], lightPositions, camera.Position);
	
		glBindVertexArray(0);
	
		glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glDisable(GL_DEPTH_TEST);
	
		shaders[ShaderType::SCREEN_SHADER].Use();
		glBindVertexArray(quadVAO);
		glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
		
	
		glfwSwapBuffers(window.m_window);
	}

	glDeleteFramebuffers(1, &framebuffer);
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