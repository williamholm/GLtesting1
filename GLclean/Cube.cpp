#include "Cube.h"

#include <string>

Cube::Cube(float objectLength, glm::vec3 pos, Shader shader, std::string texturePath):
	m_shader(shader),
	m_objectLength(objectLength),
	m_pos(pos)

{ //is this really the best way to do it???
 
	GLfloat vertexPositions[] = {


		-0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,   0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,   0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,   0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,   0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,   0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,   0.0f,  0.0f, 1.0f,   0.0f, 1.0f,

		-0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,

		0.5f,  0.5f,  0.5f,   1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,   1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,   1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,   1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,   1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,   1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	   -0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,  0.0f, 1.0f, 					 // top-right  
	   -0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,  0.0f, 0.0f, 					 // bottom-righ
		0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,  1.0f, 0.0f,  					// bottom-left
		0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,  1.0f, 0.0f,  					// bottom-left
		0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,  1.0f, 1.0f,  					// top-left 
	   -0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,  0.0f, 1.0f, 					 // top-right
		
		-0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,   0.0f, 1.0f
  };
//set up buffers
  glGenBuffers(1, &m_VBO);
  glGenVertexArrays(1, &m_VAO);


  glBindVertexArray(m_VAO);
  glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8* sizeof(GLfloat), (GLvoid*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
  glEnableVertexAttribArray(2);

  glBindVertexArray(0);

//set up texture
  glGenTextures(1, &m_texture);
  glBindTexture(GL_TEXTURE_2D, m_texture);
  
  int width, height;
  unsigned char* image = SOIL_load_image(texturePath.c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
 
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
  glGenerateMipmap(GL_TEXTURE_2D);
 
  SOIL_free_image_data(image);
  glBindTexture(GL_TEXTURE_2D, 0);


}			

Cube::Cube(float objectLength, glm::vec3 pos, Shader shader) :
	m_shader(shader),
	m_objectLength(objectLength),
	m_pos(pos)
	
{ //is this really the best way to do it???
	m_texture = 0;
	GLfloat vertexPositions[] = {
		// Back face
		-0.5f, -0.5f, -0.5f, // Bottom-left
		 0.5f, -0.5f, -0.5f, // bottom-right    
		 0.5f,  0.5f, -0.5f, // top-right              
		 0.5f,  0.5f, -0.5f, // top-right
		-0.5f,  0.5f, -0.5f, // top-left
		-0.5f, -0.5f, -0.5f, // bottom-left                
		  // Front face
		-0.5f, -0.5f,  0.5f, // bottom-left
		 0.5f,  0.5f,  0.5f, // top-right
		 0.5f, -0.5f,  0.5f, // bottom-right        
		 0.5f,  0.5f,  0.5f, // top-right
	    -0.5f, -0.5f,  0.5f, // bottom-left
	    -0.5f,  0.5f,  0.5f, // top-left        
							// Left face
		-0.5f,  0.5f,  0.5f, // top-right
		-0.5f, -0.5f, -0.5f, // bottom-left
		-0.5f,  0.5f, -0.5f, // top-left       
		-0.5f, -0.5f, -0.5f, // bottom-left
		-0.5f,  0.5f,  0.5f, // top-right
		-0.5f, -0.5f,  0.5f, // bottom-right
		 // Right face
		 0.5f,  0.5f,  0.5f, // top-left
		 0.5f,  0.5f, -0.5f, // top-right      
		 0.5f, -0.5f, -0.5f, // bottom-right          
		 0.5f, -0.5f, -0.5f, // bottom-right
		 0.5f, -0.5f,  0.5f, // bottom-left
		 0.5f,  0.5f,  0.5f, // top-left
		// Bottom face       
		-0.5f, -0.5f, -0.5f, // top-right
		-0.5f, -0.5f,  0.5f, // bottom-right
		0.5f, -0.5f,  0.5f, // bottom-left
		0.5f, -0.5f,  0.5f, // bottom-left
		0.5f, -0.5f, -0.5f, // top-left 
		-0.5f, -0.5f, -0.5f, // top-right

		// Top face
		-0.5f,  0.5f, -0.5f, // top-left
		 0.5f,  0.5f, -0.5f, // top-right
		 0.5f,  0.5f,  0.5f, // bottom-right                 
		 0.5f,  0.5f,  0.5f, // bottom-right
		-0.5f,  0.5f,  0.5f, // bottom-left  
		-0.5f,  0.5f, -0.5f 
	};
	//set up buffers
	glGenBuffers(1, &m_VBO);
	glGenVertexArrays(1, &m_VAO);


	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

}


Cube::~Cube()
{
}

void Cube::setUpStdUniforms(const glm::mat4 &view, const GLfloat &zoom)
{
	//setting up matrices

	GLint viewLoc = glGetUniformLocation(m_shader.Program, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	glm::mat4 projection;
	projection = glm::perspective(zoom, (GLfloat)800 / (GLfloat)600, 0.01f, 100.0f);

	GLint projectionLoc = glGetUniformLocation(m_shader.Program, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

	glm::mat4 model;
	GLint modelLoc = glGetUniformLocation(m_shader.Program, "model");

	model = glm::translate(model, m_pos);
	model = glm::scale(model, glm::vec3(m_objectLength));

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));



}


void Cube::draw(const glm::mat4 &view, const GLfloat &zoom)
{
	setUpStdUniforms(view, zoom);

	if (m_texture)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_texture);
	//	glUniform1i(glGetUniformLocation(m_shader.Program, "ourTexture1"), 0);
	}
	
	glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

}

void Cube::draw(const glm::mat4 &view, const GLfloat &zoom, const glm::vec3 &viewPos, const std::vector<glm::vec3> &light_pos)
{
	setUpStdUniforms(view, zoom);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	
	glUniform3f(glGetUniformLocation(m_shader.Program, "viewPos"), viewPos.x, viewPos.y, viewPos.z);

	glUniform1f(glGetUniformLocation(m_shader.Program, "material.shininess"), 16.0f);
	glUniform1i(glGetUniformLocation(m_shader.Program, "material.diffuse"), 0);
	glUniform1i(glGetUniformLocation(m_shader.Program, "material.specular"), 0);

	glUniform3f(glGetUniformLocation(m_shader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
	glUniform3f(glGetUniformLocation(m_shader.Program, "dirLight.ambient"), 0.05f, 0.05f, 0.05f);
	glUniform3f(glGetUniformLocation(m_shader.Program, "dirLight.diffuse"), 0.4f, 0.4f, 0.4f);
	glUniform3f(glGetUniformLocation(m_shader.Program, "dirLight.specular"), 0.5f, 0.5f, 0.5f);

	for (int i = 0; i < light_pos.size(); i++)
	{
		std::string iString = std::to_string(i);
		
		glUniform3f(glGetUniformLocation(m_shader.Program, std::string("pointLights[" + iString + "].position").c_str()),
			light_pos[i].x, light_pos[i].y, light_pos[i].z);
		glUniform3f(glGetUniformLocation(m_shader.Program, std::string("pointLights[" + iString + "].ambient").c_str()), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(m_shader.Program, std::string("pointLights[" + iString + "].diffuse").c_str()), 0.8f, 0.8f, 0.8f);
		glUniform3f(glGetUniformLocation(m_shader.Program, std::string("pointLights[" + iString + "].specular").c_str()), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(m_shader.Program, std::string("pointLights[" + iString + "].constant").c_str()), 1.0f);
		glUniform1f(glGetUniformLocation(m_shader.Program, std::string("pointLights[" + iString + "].linear").c_str()), 0.09f);
		glUniform1f(glGetUniformLocation(m_shader.Program, std::string("pointLights[" + iString + "].quadratic").c_str()), 0.032f);
	}

	glBindVertexArray(m_VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

}


void Cube::usingShader()
{
	m_shader.Use();
}
