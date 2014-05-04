#include "SpaceChunks.h"

#include "Shader.h"
#include "Mesh.h"

Shader* shader;
Mesh* mesh;

Vertex vertices[] = { Vertex(glm::vec3(-0.5, -0.5, 0)), 
					  Vertex(glm::vec3( 0,    0.5, 0)), 
					  Vertex(glm::vec3( 0.5, -0.5, 0))};

int main()
{
	engine->CreateWindow(800, 600, "SpaceChunks", true);

	shader = new Shader("shaders/basicShader");
	mesh = new Mesh(vertices, sizeof(vertices) / sizeof(vertices[0]));

	while (engine->Running())
	{
		shader->Bind();
		mesh->Draw();

		engine->RenderScene();
	}
	engine->DestroyWindow();

	return 0;
}

