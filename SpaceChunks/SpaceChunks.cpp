#include "XyEngine.h"
XyEngine* engine;
#include "Player.h"
#include "Chunk.h"
#include "WorldManager.h"

#include "BlockUtil.h"

bool ShowGrid = false;

bool showDebug = false;

Player *player;

WorldManager *world;

std::string spaceChunkLog = "Log...";

void Log(std::string log)
{
	spaceChunkLog = " ";
	spaceChunkLog.append(log);
}

void Init()
{
	engine->ClearScreen(0.12, 0.56, 1.0, 1.0);
	player = new Player(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), engine);
	glClearColor(0.12f, 0.56f, 1.0f, 1.0f);

	glViewport(0, 0, windowWidth, windowHeight);
	engine->Set3D();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	engine->GenSeed();
	double StartTime = Time::GetTime();
	world->UpdateSetupList();
	double EndTime = Time::GetTime() - StartTime;
	std::cout << "World Took " << EndTime << " Seconds to Load!" << std::endl;

	spaceChunkLog = "SpaceChunks Has Loaded...";
	

	printf(" \n \n \n");

	uint32_t block = BlockUtil::PackBlock(63, 104, 38);

	std::cout << "Color Data: " << block << std::endl;

	glm::vec3 blockOut = BlockUtil::GetBaseRGBFromBlock(block);

	std::cout << "Color Out: " << blockOut.r << " " << blockOut.g << " " << blockOut.b  << std::endl;
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
				player->SetPosition(glm::vec3(0, 20, 0));
				Log("[Player Name] Teleported To: X: " + engine->ConvertIntToString(0) + " Y: " + engine->ConvertIntToString(20) + " Z: " + engine->ConvertIntToString(0));
			}	
			break;

			case SDLK_i:		
				world->Dispose();
				break;		
		}
		break;
	}
}
 
void Render()
{
	engine->ClearScreen(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	engine->Set3D();
	
		player->Update(engine->IsMouseIn());

		world->Update(player->GetPosition(), player->GetRotation());

		if (showDebug)
		{
			engine->Set2D();
			printf("Text Render");
			//engine->RenderText(font, 5, 0, "Space Chunks Alpha 0.2");
			//engine->RenderText(font, 5, 20, "(C) 2014 Dominic Maas");
			//engine->RenderText(font, 5, 40, "FPS: " + engine->ConvertIntToString(engine->GetFPS()));

			//engine->RenderText(font, 5, 70, "View Distance: " + engine->ConvertIntToString(world->GetViewDistance()));
			//engine->RenderText(font, 5, 90, "Chunks: " + engine->ConvertIntToString(world->GetLoadedChunks()) + "  ( " + engine->ConvertIntToString(world->GetFrustumChunks())  + " )");

			//std::string playerPosStr = " X: " + engine->ConvertIntToString((int)player->GetPos().x) + " Y: " + engine->ConvertIntToString((int)player->GetPos().y) + " Z: " + engine->ConvertIntToString((int)player->GetPos().z);
			//engine->RenderText(font, 5, 110, "Player Position: " + playerPosStr);

			//std::string playerRotStr = " Pitch: " + engine->ConvertIntToString((int)player->getPitch()) + " Yaw: " + engine->ConvertIntToString((int)player->getYaw());
			//engine->RenderText(font, 5, 130, "Player Rotation: " + playerRotStr);

			//engine->RenderText(font, 5, 150, "Seed: " + engine->ConvertIntToString(engine->GetSeed()));

			//engine->RenderText(font, 5, windowHeight - 25, spaceChunkLog);

			//engine->RenderText(font, windowWidth - 200, windowHeight - 25, "G - Show Debug Screen");
			//engine->RenderText(font, windowWidth - 200, windowHeight - 45, "R - Render Chunk 0, 0, 0");
			//engine->RenderText(font, windowWidth - 200, windowHeight - 65, "L - TP To Chunk 0, 0, 0");
			//engine->RenderText(font, windowWidth - 200, windowHeight - 85, "Q - Set Blocks at Your Position");
		}
}

int main(int, char**)
{
	engine = new XyEngine();
	world = new WorldManager(engine);

	engine->SetFunctions(Init, Render, Input);
	engine->CreateWindow(windowWidth, windowHeight, "SpaceChunks", 120);

	delete player;
	delete world;
	delete engine;
	return 0;
}