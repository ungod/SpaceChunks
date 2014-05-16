#include "Game.h"
#include "Player.h"
#include "Chunk.h"
#include "WorldManager.h"

bool ShowGrid = false;

SDL_Thread *createWorldThread;

Player player(glm::vec3(132, 20, 132), 0, 180);
TTF_Font *font;

WorldManager world;

std::string spaceChunkLog = "Log...";

int         threadReturnValue = 0;

void Log(std::string log)
{
	spaceChunkLog = " ";
	spaceChunkLog.append(log);
}

void Init()
{
	font = TTF_OpenFont("fonts/arial.ttf", 14);
	glClearColor(0.12f, 0.56f, 1.0f, 1.0f);

	//createWorldThread = SDL_CreateThread(CreateWorld, "World Thread", (void *)NULL);

	glViewport(0, 0, windowWidth, windowHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70.0, windowWidth / windowHeight, 0.01, 1000.0);
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_CULL_FACE);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHT0);
	const GLfloat AmbientLight[] = { 0.5, 0.5, 0.5, 1.0 };	
	glLightfv(GL_LIGHT0, GL_AMBIENT, AmbientLight);
	
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	

	player.Init();

	world.CreateWorld();

	spaceChunkLog = "SpaceChunks Has Loaded...";
}

void Input(SDL_Event event)
{
	switch (event.type)
	{
	case SDL_KEYDOWN:
		switch (event.key.keysym.sym)
		{
			case SDLK_l:
			{
				player.SetPosition(glm::vec3(0, 20, 0), player.getPitch(), player.getYaw());
				Log("[Player Name] Teleported To: X: " + engine->ConvertIntToString(0) + " Y: " + engine->ConvertIntToString(20) + " Z: " + engine->ConvertIntToString(0));
			}	
			break;

			case SDLK_q:
			{
				Chunk* tmpChk = world.getChunkInWorld((int)player.GetPos().x, (int)player.GetPos().y, (int)player.GetPos().z);

				if (tmpChk != NULL)
					tmpChk->setBlock((int)player.GetPos().x, (int)player.GetPos().y, (int)player.GetPos().z, BlockType_Dirt);
			}
			break;

			case SDLK_g:
			{
				ShowGrid = !ShowGrid;
			}	
			break;

			case SDLK_r:
			{
				Chunk* tmpChk1 = world.getChunkInWorld((int)player.GetPos().x, (int)player.GetPos().y, (int)player.GetPos().z);

				if (tmpChk1 != NULL)
				{
					if (tmpChk1->isChunkRendered())
						tmpChk1->setChunkRendered(false);
					else
						tmpChk1->setChunkRendered(true);
				}
				else
				{
					Log("Chunk Not Here!");
				}
				break;
			}
				
		}
		break;
	}
}
 
void Render()
{
	engine->ClearScreen(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	engine->Set3D();
	
		player.Update(engine->IsMouseIn());
		const GLfloat PositonLight[] = { 0.5, 0.0, -0.5, 0 };
		glLightfv(GL_LIGHT0, GL_POSITION, PositonLight);
		world.Update();

	engine->Set2D();
		engine->RenderText(font, 5, 0, "Space Chunks Alpha 0.2");
		engine->RenderText(font, 5, 20, "(C) 2014 Dominic Maas");
		engine->RenderText(font, 5, 40, "FPS: " + engine->ConvertIntToString(engine->GetFPS()));

		engine->RenderText(font, 5, 70, "Chunk Updates: 0");
		engine->RenderText(font, 5, 90, "Chunk Rendered: " + engine->ConvertIntToString(m_chunksLoaded));

		std::string playerPosStr = " X: " + engine->ConvertIntToString((int)player.GetPos().x) + " Y: " + engine->ConvertIntToString((int)player.GetPos().y) + " Z: " + engine->ConvertIntToString((int)player.GetPos().z);
		engine->RenderText(font, 5, 110, "Player Position: " + playerPosStr);

		std::string playerRotStr = " Pitch: " + engine->ConvertIntToString((int)player.getPitch()) + " Yaw: " + engine->ConvertIntToString((int)player.getYaw());
		engine->RenderText(font, 5, 130, "Player Rotation: " + playerRotStr);

		engine->RenderText(font, 5, 150, "Seed: " + engine->ConvertIntToString(engine->GetSeed()));

		engine->RenderText(font, 5, windowHeight - 25, spaceChunkLog);

		engine->RenderText(font, windowWidth - 200, windowHeight - 25, "G - Show Chunk Grid");
		engine->RenderText(font, windowWidth - 200, windowHeight - 45, "R - Render Chunk 0, 0, 0");
		engine->RenderText(font, windowWidth - 200, windowHeight - 65, "L - TP To Chunk 0, 0, 0");
		engine->RenderText(font, windowWidth - 200, windowHeight - 85, "Q - Set Blocks at Your Position");
}

int main(int, char**)
{
	engine->CreateWindow(windowWidth, windowHeight, "SpaceChunks", 30, Init, Render, Input);

	world.DisposeWorld();
	engine->DestroyWindow();

	return 0;
}