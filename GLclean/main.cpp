#define GLEW_STATIC
#include "Cube.h"
#include "Camera.h"
#include "Window.h"
#include "MasterRenderer.h"

void keyCallBack(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouseCallBack(GLFWwindow* window, double xpos, double ypos);
void scrollCallBack(GLFWwindow* window, double xoffset, double yoffset);
void mouseButtonCallBack(GLFWwindow* window, int key, int action, int mode);
void do_movement();
void placeBlock();
GLuint generateAttachmentTexture(GLboolean depth, GLboolean stencil);

const GLuint WIDTH = 800, HEIGHT = 600;

// Camera
Camera  camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;

bool    keys[1024];

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

#pragma region setKeyCallBacks
	glfwSetKeyCallback(window.m_window, keyCallBack);
	glfwSetCursorPosCallback(window.m_window, mouseCallBack);
	glfwSetScrollCallback(window.m_window, scrollCallBack);
	glfwSetMouseButtonCallback(window.m_window, mouseButtonCallBack);
#pragma endregion

#pragma region shaders



	shaders.emplace_back(Shader("vertShaderLight.txt", "fragShaderLight.txt"));
	shaders.emplace_back(Shader("vertObjShader.txt", "fragLightObjShader.txt"));
	shaders.emplace_back(Shader("vertModel.txt", "fragModel.txt"));
	shaders.emplace_back(Shader("vertexShader.txt", "singleColourFrag.txt"));
	shaders.emplace_back(Shader("vertSimple.txt", "fragSimple.txt"));
	shaders.emplace_back(Shader("vertNoTextureModel.txt", "fragNoTextureModel.txt"));
#pragma endregion

	lights.emplace_back(Cube(0.1, glm::vec3(0, 0, 0), shaders[ShaderType::LIGHTOBJ]));
	lightPositions.emplace_back(glm::vec3(0, 0, 0));
//note nanosuit is ~~13k vertices
	Model g("ModelStuff/NanoSluit/simple_sword2.obj",glm::vec3(0,0,0),glm::vec3(1,1,1));
	Model person("ModelStuff/NanoSluit/C1.obj", glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));

	Terrain terrain;
	MasterRenderer renderMaster;
	renderMaster.loadTerrain(&terrain);
	glm::mat4x4 view;
	glm::mat4x4 projection;

	float z = 5 * camera.Front.z;
#pragma region Render/Framebuffer
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
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, WIDTH, HEIGHT); // Use a single renderbuffer object for both a depth AND stencil buffer.
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); //  attach it. completed the framebuffer and  have 
																							//added all attachments. check if it is complete 
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	for (int i = 0; i < 3; i++)
	{
		g.updatePosition(glm::vec3(5*i, 0, 5*i));
		renderMaster.addModel(&g);
	}
	

#pragma endregion

	while (!glfwWindowShouldClose(window.m_window))
	{	
		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
	//	std::cout << 1 / deltaTime << "\n";
		std::cout << "(" << person.m_position.x << ", " << person.m_position.y << ", " << person.m_position.z<<")\n";
	
		glfwPollEvents();
		do_movement();
		placeBlock();

		projection = glm::perspective(camera.Zoom, (GLfloat)WIDTH/(GLfloat)HEIGHT, 0.01f, 100.0f);		
		view = camera.GetViewMatrix();		
	
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer); //Draw normally here		
		glClearColor(0.1f, 0.3f, 0.7f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 	
		glEnable(GL_DEPTH_TEST);		

		lights[0].usingShader();
		for (int i = 0; i < lights.size() ; i++)
		{
			lights[i].draw(view, camera.Zoom);
		}


		person.updatePosition(glm::vec3(camera.Position.x, renderMaster.m_terrain->getHeight(person.m_position.x, person.m_position.z), camera.Position.z + z));


		renderMaster.updateShaderInfo(view, projection, camera.Position, lightPositions);
		renderMaster.drawModels(shaders[ShaderType::MODEL_C]);
		person.Draw(shaders[ShaderType::MODEL_C]);
		glBindVertexArray(0);
	
		glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default, draw quad.
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