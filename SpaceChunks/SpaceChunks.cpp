#include "Game.h"

#include "Shader.h"
#include "Player.h"
#include "Chunk.h"
#include <sstream>
bool chunkRen = false;

Shader* shader;
Chunk* chunk;
Chunk* chunk2;
Player player(glm::vec3(5, 17, 5), 0, 180);

float frameRate = 60;

float m_frameTime = 1.0 / frameRate;

bool mousein = false;

SDL_Color color;
SDL_Rect position;

TTF_Font *font;

int frames = 0;

int framerate;

void Init()
{
	chunk = new Chunk(0, 0, 0);
	chunk2 = new Chunk(16, 0, 0);
	shader = new Shader("shaders/basicShader");

	//GLint uniColor = engine->GetShaderUniform(shader->GetShaderProgram(), "inputColour");


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
	glLightfv(GL_LIGHT0, GL_DIFFUSE, DiffuseLight);
	glLightfv(GL_LIGHT0, GL_AMBIENT, AmbientLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, SpecularLight);


	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	player.Init();

	chunk->CreateChunk();
	chunk2->CreateChunk();
}

void Make3D()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity(); 
	gluPerspective(60.0, windowWidth / windowHeight, 0.01, 1000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_DEPTH_TEST);
}

void Make2D()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, windowWidth, windowHeight, 0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_CULL_FACE);
}

void Render()
{

	engine->ClearScreen(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	float pos[] = { 0.4, -1.0, 0.4, 0.4 };
	glLightfv(GL_LIGHT0, GL_POSITION, pos);

	Make3D();

	player.Update(mousein);

	if (mousein)
		SDL_WarpMouseInWindow(engine->GetWindow(), MidX, MidY);

	
	chunk->UpdateChunk();
	chunk2->UpdateChunk();


	Make2D();

	color.r = 255;
	color.g = 255;
	color.b = 255;

	position.x = 5;
	position.y = 0;

	std::string fpsRen;

	std::ostringstream con;

	con << framerate;

	fpsRen = con.str();


	engine->RenderText(font, 255, 255, 255, 5, 0, 0, "Space Chunks Alpha 0.2");
	engine->RenderText(font, 255, 255, 255, 5, 20, 0, "(C) 2014 Dominic Maas");
	engine->RenderText(font, 255, 255, 255, 5, 40, 0, "FPS: " + fpsRen);

	glm::vec3 playerPos = player.GetPos();
	std::ostringstream con2;
	std::string px;

	con2 << " X: " << (int)playerPos.x << " Y: " << (int)playerPos.y << " Z: " << (int)playerPos.z;

	px = con2.str();

	
	engine->RenderText(font, 255, 255, 255, 5, 60, 0, "Player Position: " + px);

	std::ostringstream con3;
	std::string py;

	con3 << " Pitch: " << (int)player.getPitch() << " Yaw: " << (int)player.getYaw();

	py = con3.str();

	engine->RenderText(font, 255, 255, 255, 5, 80, 0, "Player Rotation: " + py);
}

int main(int, char**)
{
	engine->CreateWindow(windowWidth, windowHeight, "SpaceChunks", false, 16);

	TTF_Init();

	if (!IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG))
	{
		printf("IMG_Init: Failed to init required jpg and png support!\n");
		printf("IMG_Init: %s\n", IMG_GetError());
	}

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
			std::cout << frames << std::endl;
			framerate = frames;
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

