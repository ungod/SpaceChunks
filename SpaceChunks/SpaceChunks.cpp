#include "Game.h"

#include "Player.h"
#include "Chunk.h"
#include <sstream>
bool chunkRen = false;

Chunk* chunk;
Player player(glm::vec3(5, 17, 5), 0, 180);

float frameRate = 60;

float m_frameTime = 1.0 / frameRate;

bool mousein = false;

TTF_Font *font;

int frames = 0;

int fpsRate;

void Init()
{
	chunk = new Chunk(0, 0, 0);

	font = TTF_OpenFont("fonts/font.ttf", 18);
	glClearColor(0.12f, 0.56f, 1.0f, 1.0f);

	glViewport(0, 0, windowWidth, windowHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, windowWidth / windowHeight, 0.01, 1000.0);
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_CULL_FACE);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHT0);
	const GLfloat DiffuseLight[] = { 0.5f, 0.5f, 0.5f, 0.5f };
	const GLfloat AmbientLight[] = { 0.5f, 0.5f, 0.5f, 0.5f };
	const GLfloat SpecularLight[] = { 0.5f, 0.5f, 0.5f, 0.5f };
	const GLfloat PositonLight[] = { 0.4, -1.0, 0.4, 0.4 };
	
	glLightfv(GL_LIGHT0, GL_DIFFUSE, DiffuseLight);
	glLightfv(GL_LIGHT0, GL_AMBIENT, AmbientLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, SpecularLight);
	glLightfv(GL_LIGHT0, GL_POSITION, PositonLight);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	player.Init();

	chunk->CreateChunk();
}

void Render()
{
	/* New Render System (Upgrading) */

	engine->ClearScreen(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	engine->Set3D();
		player.Update(mousein);
		chunk->UpdateChunk();

	engine->Set2D();
		engine->RenderText(font, 5, 0, "Space Chunks Alpha 0.2");
		engine->RenderText(font, 5, 20, "(C) 2014 Dominic Maas");
		engine->RenderText(font, 5, 40, "FPS: " + engine->ConvertIntToString(fpsRate));

		engine->RenderText(font, 5, 80, "Chunk Updates: 0");
		engine->RenderText(font, 5, 100, "Chunk Rendered: 1");

		glm::vec3 playerPos = player.GetPos();

		std::string playerPosStr = " X: " + engine->ConvertIntToString((int)playerPos.x) + " Y: " + engine->ConvertIntToString((int)playerPos.y) + " Z: " + engine->ConvertIntToString((int)playerPos.z);
		engine->RenderText(font, 5, windowHeight - 45, "Player Position: " + playerPosStr);

		std::string playerRotStr = " Pitch: " + engine->ConvertIntToString((int)player.getPitch()) + " Yaw: " + engine->ConvertIntToString((int)player.getYaw());

		engine->RenderText(font, 5, windowHeight - 25, "Player Rotation: " + playerRotStr);
}

int main(int, char**)
{
	engine->CreateWindow(windowWidth, windowHeight, "SpaceChunks", false);

	bool running = true;

	double lastTime = Time::GetTime();
	double unprocessedTime = 0;
	double frameCounter = 0;
	frames = 0;

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
			fpsRate = frames;
			frames = 0;
			frameCounter = 0;
		}

		while (unprocessedTime > m_frameTime)
		{
			render = true;

			

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
					case SDLK_0:
						chunk->setBlock(0, 0, 0, 0);
						break;
					case SDLK_1:
						chunk->setBlock(0, 0, 0, 1);
						break;
					case SDLK_m:
						chunkRen = !chunkRen;
						break;
					}
				}
			}

			unprocessedTime -= m_frameTime;
		}

		if (render)
		{
			if (mousein)
				SDL_WarpMouseInWindow(engine->GetWindow(), MidX, MidY);

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

	delete chunk;

	engine->DestroyWindow();

	return 0;
}

