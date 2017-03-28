#include "MasterRenderer.h"



MasterRenderer::MasterRenderer()
{
}


MasterRenderer::~MasterRenderer()
{
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
