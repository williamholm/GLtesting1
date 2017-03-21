//#define GLEW_STATIC
//#include <GL\glew.h>
//#include <GLFW\glfw3.h>
//#include <iostream>
//#include <string>
//#include <fstream>
//#include "Shader.h"
//#include <src/SOIL.h>
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
////===============================================================================\\
// //VERTEX SHADER////////
///*
//#version 330 core
//layout(location = 0) in vec3 position;
//layout(location = 1) in vec3 color;
//layout(location = 2) in vec2 texCoord;
//
//uniform mat4 transform;
//
//out vec3 ourColor;
//out vec2 TexCoord;
//
//void main()
//{
//	gl_Position = transform * vec4(position, 1.0f);
//	ourColor = color;
//	TexCoord = vec2(texCoord.x, 1 - texCoord.y);
//}
//
/////FRAGMENT SHADER////
//
//#version 330 core
//in vec3 ourColor;
//in vec2 TexCoord;
//
//out vec4 color;
//
//uniform sampler2D ourTexture1;
//uniform sampler2D ourTexture2;
//
//void main()
//{
//color = mix(texture(ourTexture1, TexCoord), texture(ourTexture2, TexCoord), 0.2);
//}
//
////=================================================================================*/
//
//void keyCallBack(GLFWwindow* window, int key, int scancode, int action, int mode)
//{
//	// When a user presses the escape key, we set the WindowShouldClose property to true, 
//	// closing the application
//	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
//		glfwSetWindowShouldClose(window, GL_TRUE);
//}
//
//
//
//int main()
//{
//
//
//	////////////////////////////////////BASIC SET UP OF GLFW AND GLEW////////////////////////
//	glfwInit();
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
//
//
//	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
//	glfwMakeContextCurrent(window);
//
//	if (window == nullptr)
//	{
//		std::cout << "Failed to create GLFW window" << std::endl;
//		glfwTerminate();
//		return -1;
//	}
//
//	glewExperimental = GL_TRUE; //allows more mordern opengl to be used
//	if (glewInit() != GLEW_OK)
//	{
//		std::cout << "glew failed to init\n";
//		return -1;
//	}
//
//	int fb_width, fb_height;
//	glfwGetFramebufferSize(window, &fb_width, &fb_height); //width set to window width, height set to height width.
//	glViewport(0, 0, fb_width, fb_height); //tells open gl where to render too.   0,0 sets the start location to lower left corner of window!
//
//										   //////////////////////////////SET UP GRAPHICS STUFFS//////////////////////////////////////////////////////////////
//
//	Shader ourShader("vertexShader.txt", "fragmentShader.txt");
//
//
//
//
//	GLfloat vertices[] = {
//		//position xyz		 color rgb			TextCoords
//		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,	1.0f, 1.0f,  // Top Right
//		0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,	1.0f, 0.0f,	 // Bottom Right
//		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,	0.0f, 0.0f,  // Bottom Left
//		-0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,	0.0f, 1.0f	 // Top Left 
//	};
//	GLuint indices[] = {  // Note that we start from 0!
//		0, 1, 3,   // First Triangle
//		1, 2, 3    // Second Triangle
//	};
//
//
//	GLuint VBO, VAO;
//	glGenVertexArrays(1, &VAO);
//	glGenBuffers(1, &VBO);
//	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
//	glBindVertexArray(VAO);
//
//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//
//	GLuint EBO;
//	glGenBuffers(1, &EBO);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
//
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
//	glEnableVertexAttribArray(0);
//	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
//	glEnableVertexAttribArray(1);
//	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
//	glEnableVertexAttribArray(2);
//
//	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)
//
//
//						  // Load and create a texture 
//	GLuint texture1;
//	GLuint texture2;
//	// ====================
//	// Texture 1
//	// ====================
//	glGenTextures(1, &texture1);
//	glBindTexture(GL_TEXTURE_2D, texture1); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object
//											// Set our texture parameters
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//	// Set texture filtering
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	// Load, create texture and generate mipmaps
//	int width, height;
//	unsigned char* image = SOIL_load_image("container.jpg", &width, &height, 0, SOIL_LOAD_RGB);
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
//	glGenerateMipmap(GL_TEXTURE_2D);
//	SOIL_free_image_data(image);
//	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.
//									 // ===================
//									 // Texture 2
//									 // ===================
//	glGenTextures(1, &texture2);
//	glBindTexture(GL_TEXTURE_2D, texture2);
//	// Set our texture parameters
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//	// Set texture filtering
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	// Load, create texture and generate mipmaps
//	image = SOIL_load_image("awesomeface.png", &width, &height, 0, SOIL_LOAD_RGB);
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
//	glGenerateMipmap(GL_TEXTURE_2D);
//	SOIL_free_image_data(image);
//	glBindTexture(GL_TEXTURE_2D, 0);
//
//	////////////////////////////GAME LOOP//////////////////////////////////////////
//
//	while (!glfwWindowShouldClose(window))
//	{
//		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
//		glfwPollEvents();
//
//		// Render
//		// Clear the colorbuffer
//		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
//		glClear(GL_COLOR_BUFFER_BIT);
//
//
//		glm::mat4 trans;
//		trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
//		trans = glm::rotate(trans, (GLfloat)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.0f, 0.0f, 1.0f));
//
//		GLuint transformLoc = glGetUniformLocation(ourShader.Program, "transform");
//		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
//
//
//		// Activate shader
//		ourShader.Use();
//
//
//		glActiveTexture(GL_TEXTURE0);
//		glBindTexture(GL_TEXTURE_2D, texture1);
//		glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture1"), 0);
//		glActiveTexture(GL_TEXTURE1);
//		glBindTexture(GL_TEXTURE_2D, texture2);
//		glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture2"), 1);
//
//		// Draw container
//		glBindVertexArray(VAO);
//		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
//
//
//
//
//
//		glm::mat4 transx;
//		transx = glm::translate(transx, glm::vec3(0.5f, 0.5f, 0.0f));
//		transx = glm::rotate(transx, (GLfloat)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.0f, 0.0f, 1.0f));
//
//
//		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transx));
//		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
//		glBindVertexArray(0);
//
//		// Swap the screen buffers
//		glfwSwapBuffers(window);
//	}
//	// Properly de-allocate all resources once they've outlived their purpose
//	glDeleteVertexArrays(1, &VAO);
//	glDeleteBuffers(1, &VBO);
//	glDeleteBuffers(1, &EBO);
//	// Terminate GLFW, clearing any resources allocated by GLFW.
//	glfwTerminate();
//	return 0;
//
//	// Properly de-allocate all resources once they've outlived their purpose
//	glDeleteVertexArrays(1, &VAO);
//	glDeleteBuffers(1, &VBO);
//
//	glfwTerminate(); //cleans up all memory allocated to glfw
//	return 0;
//}
//