#include "Game.h"

#include "Shader.h"
#include "Mesh.h"

Shader* shader;
Mesh* mesh;

Vertex vertices[] = { Vertex(glm::vec3( 0.0f,  0.5f, 0.0)), 
					  Vertex(glm::vec3( 0.5f, -0.5f, 0.0)), 
					  Vertex(glm::vec3(-0.5f, -0.5f, 0.0))};

float matrix[] = {
	1.0f, 0.0f, 0.0f, 0.0f, // first column
	0.0f, 1.0f, 0.0f, 0.0f, // second column
	0.0f, 0.0f, 1.0f, 0.0f, // third column
	0.5f, 0.0f, 0.0f, 1.0f // fourth column
};

int main()
{
	engine->CreateWindow(800, 600, "SpaceChunks", true, 16);

	shader = new Shader("shaders/basicShader");
	mesh = new Mesh(vertices, sizeof(vertices) / sizeof(vertices[0]));

	GLint uniColor = engine->GetShaderUniform(shader->GetShaderProgram(), "inputColour");

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	float speed = 0.3f; // move at 1 unit per second
	float last_position = 0.0f;

	while (engine->Running())
	{		
		int matrix_location = engine->GetShaderUniform(shader->GetShaderProgram(), "matrix");

		static double previous_seconds = glfwGetTime();
		double current_seconds = glfwGetTime();
		double elapsed_seconds = current_seconds - previous_seconds;
		previous_seconds = current_seconds;

		// reverse direction when going to far left or right
		if (fabs(last_position) > 1.0f) {
			speed = -speed;
		}

		// update the matrix
		matrix[12] = elapsed_seconds * speed + last_position;
		last_position = matrix[12];









		

		glUniformMatrix4fv(matrix_location, 1, GL_FALSE, matrix);

		engine->SetShaderUniform(uniColor, 0.0f, 0.8f, 0.2f, 0.0f);

		shader->Bind();
		mesh->Draw();

		engine->RenderScene();
	}
	engine->DestroyWindow();

	return 0;
}

