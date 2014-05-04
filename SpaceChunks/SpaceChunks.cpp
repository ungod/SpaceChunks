#include "Game.h"

#include "Shader.h"
#include "Mesh.h"

Shader* shader;
Mesh* mesh;

Vertex vertices[] = { Vertex(glm::vec3(-0.5, -0.5, 0.0)), 
					  Vertex(glm::vec3(-0.5,  0.5, 0.0)), 
					  Vertex(glm::vec3( 0.5,  0.5, 0.0))};

int main()
{
	engine->CreateWindow(800, 600, "SpaceChunks", true);

	shader = new Shader("shaders/basicShader");
	mesh = new Mesh(vertices, sizeof(vertices) / sizeof(vertices[0]));

	GLint uniColor = engine->GetShaderUniform(shader->GetShaderProgram(), "triangleColor");

	while (engine->Running())
	{		
		engine->SetShaderUniform(uniColor, 0.2f, 0.3f, 0.8f);

		shader->Bind();
		mesh->Draw();

		engine->RenderScene();
	}
	engine->DestroyWindow();

	return 0;
}

