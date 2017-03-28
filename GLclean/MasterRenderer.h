#pragma once
#include "Model.h"
#include <map>
#include "Terrain.h"
// if not same assmue is different. Model position / scale is always considered to be different


class MasterRenderer
{
public:
	MasterRenderer();
	~MasterRenderer();


	void addModel(Model* model);
	//note that light_pos might be better suited to being moved elsewhere - no need to update every game loop.
	void updateShaderInfo(glm::mat4x4 view, glm::mat4x4 projection, glm::vec3 viewPos, vector<glm::vec3> light_pos);
	void drawModels(Shader shader);

	void loadTerrain(Terrain* terrain);
	void drawTerrain(Shader shader);
	Terrain* m_terrain;
private:
	vector<glm::vec3> m_light_pos;
	glm::vec3 m_viewPos;
	glm::mat4x4 m_view, m_projection;

	//vector of same model, same texture, same shader;
	vector<vector<Model*>> m_sameTextureSameModels;
	//vector of same models, different textures, same shader
	vector<vector<Model*>> m_sameModels;
	//general case
	vector<Model*> m_models;
	//terrain
};

