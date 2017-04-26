#pragma once
#include "Model.h"
#include <map>
#include "Terrain.h"
//first work on a model sorting algorithm to make drawing with large amounts of models feasable, then
//sort out drawing system in draw sequence function.
///To be answered: what are the possible things that could change the shaders being used (for models)? 
///possible things are: distance, alpha mixing or glowing effects(maybe this can be done in a particle engine tho)
//Draw distance -> variable which defines what needs to be drawn and what doesn't. perhaps also look into stopping what
//ins't in the camera's view not being drawn. 
enum ShaderType
{
	LIGHTING,
	LIGHTOBJ,
	MODEL,
	SINGLECOLOUR,
	SCREEN_SHADER,
	MODEL_C
};

class MasterRenderer
{
public:
	MasterRenderer();
	~MasterRenderer();

	Terrain* m_terrain;

	GLuint m_textureColourBuffer;
	GLuint m_quadVAO;
	GLuint m_FBDrawAsQuad;

	vector<Shader> m_shaders;

	void addModel(Model* model);
	//note that light_pos might be better suited to being moved elsewhere - no need to update every game loop.
	void updateShaderInfo(glm::mat4x4 view, glm::mat4x4 projection, glm::vec3 viewPos, vector<glm::vec3> light_pos);

	void drawModels(Shader shader);
	void drawSequence();

	void loadTerrain(Terrain* terrain);
	void drawTerrain(Shader shader);
	GLuint generateAttachmentTexture(GLboolean depth, GLboolean stencil);
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

	void initShaders();

	void readyDrawingProccess();
	void drawAsSingleQuad(Shader shader);
	void initDrawAsQuad();
};

