#include "MasterRenderer.h"
int WIDTH = 800;
int HEIGHT = 600;


MasterRenderer::MasterRenderer()
{
	initShaders();
	initDrawAsQuad();
}


MasterRenderer::~MasterRenderer()
{
}

void MasterRenderer::initShaders()
{
	m_shaders.emplace_back(Shader("vertShaderLight.txt", "fragShaderLight.txt"));
	m_shaders.emplace_back(Shader("vertObjShader.txt", "fragLightObjShader.txt"));
	m_shaders.emplace_back(Shader("vertModel.txt", "fragModel.txt"));
	m_shaders.emplace_back(Shader("vertexShader.txt", "singleColourFrag.txt"));
	m_shaders.emplace_back(Shader("vertSimple.txt", "fragSimple.txt"));
	m_shaders.emplace_back(Shader("vertNoTextureModel.txt", "fragNoTextureModel.txt"));
}

void MasterRenderer::drawSequence()
{
	readyDrawingProccess();

	drawModels(m_shaders[ShaderType::MODEL_C]);
	//drawModels Method 2...

	drawAsSingleQuad(m_shaders[ShaderType::SCREEN_SHADER]);
}

void MasterRenderer::initDrawAsQuad()
{
	GLfloat vertexPositions[] = {
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		1.0f, -1.0f,  1.0f, 0.0f,
		1.0f,  1.0f,  1.0f, 1.0f
	};
	//bad way of setting m_vertexPositions.

	GLuint quadVBO;
	glGenVertexArrays(1, &m_quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(m_quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), &vertexPositions, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
	glBindVertexArray(0);

	// Framebuffers
	glGenFramebuffers(1, &m_FBDrawAsQuad);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBDrawAsQuad);
	// Create a color attachment texture
	m_textureColourBuffer = generateAttachmentTexture(false, false);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_textureColourBuffer, 0);
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

}

GLuint MasterRenderer::generateAttachmentTexture(GLboolean depth, GLboolean stencil)
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

void MasterRenderer::readyDrawingProccess()
{
	//set up drawing to quad
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBDrawAsQuad);
	glClearColor(0.1f, 0.3f, 0.7f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
}

void MasterRenderer::drawModels(Shader shader)
{
	shader.Use();
	
	glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(m_view));
	glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(m_projection));

	glUniform3f(glGetUniformLocation(shader.Program, "viewPos"), m_viewPos.x, m_viewPos.y, m_viewPos.z);
	glUniform3f(glGetUniformLocation(shader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
	glUniform3f(glGetUniformLocation(shader.Program, "dirLight.ambient"), 0.05f, 0.05f, 0.05f);
	glUniform3f(glGetUniformLocation(shader.Program, "dirLight.diffuse"), 0.8f, 0.8f, 0.8f);
	glUniform3f(glGetUniformLocation(shader.Program, "dirLight.specular"), 0.5f, 0.5f, 0.5f);

	for (int i = 0; i < m_light_pos.size(); i++)
	{
		std::string iString = std::to_string(i);
		glUniform3f(glGetUniformLocation(shader.Program, std::string("pointLights[" + iString + "].position").c_str()),
			m_light_pos[i].x, m_light_pos[i].y, m_light_pos[i].z);
		glUniform3f(glGetUniformLocation(shader.Program, std::string("pointLights[" + iString + "].ambient").c_str()), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(shader.Program, std::string("pointLights[" + iString + "].diffuse").c_str()), 0.8f, 0.8f, 0.8f);
		glUniform3f(glGetUniformLocation(shader.Program, std::string("pointLights[" + iString + "].specular").c_str()), 1.0f, 1.0f, 1.0f);
	}
	m_terrain->m_terrainModel.Draw(shader);

	for (int i = 0; i < m_models.size(); i++)
	{		
			m_models[i]->Draw(shader);
	}

}

void MasterRenderer::drawAsSingleQuad(Shader shader)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default, draw quad.
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);

	shader.Use();
	glBindVertexArray(m_quadVAO);
	glBindTexture(GL_TEXTURE_2D, m_textureColourBuffer);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void MasterRenderer::addModel(Model* model)
{
	m_models.push_back(model);
}
void MasterRenderer::updateShaderInfo(glm::mat4x4 view, glm::mat4x4 projection, glm::vec3 viewPos, vector<glm::vec3> light_pos)
{
	m_view = view;
	m_projection = projection;
	m_viewPos = viewPos;
	m_light_pos = light_pos;
}

void MasterRenderer::loadTerrain(Terrain* terrain)
{
	m_terrain = terrain;
}
