#pragma once
#include <glm\glm.hpp>
#include <GL\glew.h>
#include "Shader.h"
#include <src/SOIL.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
class Cube
{
private:
	float m_objectLength;
	glm::vec3 m_pos;
	Shader m_shader;
	GLuint m_VBO;
	GLuint m_VAO;
	GLuint m_texture;
	glm::mat4 m_model;

public:
	Cube(float objectLength, glm::vec3 pos, Shader shader, std::string texturePath);
	Cube(float objectLength, glm::vec3 pos, Shader shader);
	~Cube();

	Shader returnShader() { return m_shader; }
	glm::vec3 returnPos() { return m_pos; }
	void setUpStdUniforms(const glm::mat4 &view, const GLfloat &zoom);
	void usingShader();
	void draw(const glm::mat4 &view, const GLfloat &zoom);
	void draw(const glm::mat4 &view,const GLfloat &zoom, const glm::vec3 &viewPos,const std::vector<glm::vec3> &light_pos);

};

