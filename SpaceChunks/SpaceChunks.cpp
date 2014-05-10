#include "Game.h"

#include "Shader.h"
#include "Player.h"
#include "Chunk.h"

Shader* shader;
Chunk* chunk;
Player player(glm::vec3(0, 0, 0));

float frameRate = 75;

float m_frameTime = 1.0 / frameRate;

bool mousein = false;

void Init()
{
	chunk = new Chunk(glm::vec3(0, 0, 0));
	shader = new Shader("shaders/basicShader");

	//GLint uniColor = engine->GetShaderUniform(shader->GetShaderProgram(), "inputColour");

	glClearColor(0.12f, 0.56f, 1.0f, 1.0f);

	glViewport(0, 0, 1280, 720);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(67.5, (float)1280 / (float)720, 0.1f, 1000.0f);
	glOrtho(0, 0, 720, 1280, 0.1, 1000);
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_CULL_FACE);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHT0);
	const GLfloat DiffuseLight[] = { 0.5f, 0.5f, 0.5f, 0.5f };
	const GLfloat AmbientLight[] = { 0.5f, 0.5f, 0.5f, 0.5f };
	const GLfloat SpecularLight[] = { 0.5f, 0.5f, 0.5f, 0.5f };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, DiffuseLight);
	glLightfv(GL_LIGHT0, GL_AMBIENT, AmbientLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, SpecularLight);

	player.Init();

	chunk->CreateChunk();
}

void Render()
{
	glDepthMask(GL_TRUE);
	engine->ClearScreen(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	float pos[] = { 0.4, -1.0, 0.4, 0.4 };
	glLightfv(GL_LIGHT0, GL_POSITION, pos);

	glLoadIdentity();

	player.Update(mousein);

	if (mousein)
		SDL_WarpMouseInWindow(engine->GetWindow(), 1280 / 2, 720 / 2);

	

	chunk->UpdateChunk();
}

int main(int, char**)
{
	engine->CreateWindow(1280, 720, "SpaceChunks", false, 16);

	if (!IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG))
	{
		printf("IMG_Init: Failed to init required jpg and png support!\n");
		printf("IMG_Init: %s\n", IMG_GetError());
	}

	bool running = true;

	double lastTime = Time::GetTime();
	double unprocessedTime = 0;
	double frameCounter = 0;
	int frames = 0;

	SDL_Event event;

	Init();

	while (running)
	{
		bool render = false;

		double startTime = Time::GetTime();
		double passedTime = startTime - lastTime;
		lastTime = startTime;

		unprocessedTime += passedTime;
		frameCounter += passedTime;

		if (frameCounter >= 1.0)
		{
			frames = 0;
			frameCounter = 0;
		}

		while (unprocessedTime > m_frameTime)
		{
			render = true;
			//Update();

			while (SDL_PollEvent(&event))
			{
				switch (event.type)
				{

				case SDL_QUIT:
					running = false;
					break;

				case SDL_MOUSEBUTTONDOWN:
					mousein = true;
					SDL_ShowCursor(SDL_DISABLE);
					break;

				case SDL_KEYDOWN:
					switch (event.key.keysym.sym)
					{
					case SDLK_ESCAPE:
						mousein = false;
						SDL_ShowCursor(SDL_ENABLE);
						break;
					}
				}
			}

			unprocessedTime -= m_frameTime;
		}

		if (render)
		{
			Render();
			SDL_GL_SwapWindow(engine->GetWindow());
			frames++;
		}
		else
		{
			SDL_Delay(1);
		}
	}


	chunk->DisposeChunk();

	delete shader;
	delete chunk;

	engine->DestroyWindow();

	return 0;
}

