#include "XyEngine.h"
XyEngine* engine;

#include "Player.h"
#include "Chunk.h"
#include "WorldManager.h"


Player *player;
WorldManager *world;

bool isDebug = true;
bool enableLighting = true;


void Init()
{
	engine->ClearScreen(0.12f, 0.56f, 1.0f, 1.0f);
	player = new Player(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), engine);

	glClearColor(0.12f, 0.56f, 1.0f, 1.0f);

	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_MULTISAMPLE_ARB);

	glViewport(0, 0, (GLsizei)engine->GetWindowWidth(), (GLsizei)engine->GetWindowHeight());
	engine->Set3D();

	engine->GenSeed();

	double StartTime = Time::GetTime();
	world->UpdateSetupList();
	double EndTime = Time::GetTime() - StartTime;
	std::cout << "World Took " << EndTime << " Seconds to Load!" << std::endl;
}

void Input(SDL_Event event)
{
	switch (event.type)
	{
	case SDL_KEYDOWN:
		switch (event.key.keysym.sym)
		{
			case SDLK_l:		
				player->SetPosition(glm::vec3(0, 64, 0));		
				break;
	
			case SDLK_F3:
				isDebug = !isDebug;
				break;
			case SDLK_F4:
				enableLighting = !enableLighting;
				break;
			case SDLK_q:
				world->SetBlock(player->GetPosition().x, player->GetPosition().y, player->GetPosition().z, BlockType::Grass);
				break;
			case SDLK_e:
				world->SetBlock(player->GetPosition().x, player->GetPosition().y - 1, player->GetPosition().z, BlockType::Air);
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
		
		GLfloat ambient_color[] = { 0.7f, 0.7f, 0.7f, 0.7f };
		GLfloat light_position[] = { 1.0f, 1.0f, 1.0f, 0.0f };

		glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_color);
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);

		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glShadeModel(GL_SMOOTH);

		

		if (isDebug)
		{
			engine->Set2D();
				engine->RenderText(5, 0, "Space Chunks Alpha 0.3.1");
				engine->RenderText(5, 15, "© 2014 - 2015 Dominic Maas");

				engine->RenderText(5, 45, "Render FPS: " + engine->ConvertIntToString((int)engine->GetFPS()));
				engine->RenderText(5, 60, "Physics FPS: " + engine->ConvertIntToString((int)engine->GetPhysicsFPS()));

				engine->RenderText(5, 90, "View Distance: " + engine->ConvertIntToString(world->GetViewDistance()));
				engine->RenderText(5, 105, "Chunks: " + engine->ConvertIntToString(world->GetLoadedChunks()) + "  (" + engine->ConvertIntToString(world->GetFrustumChunks()) + ")");

				std::string playerPosStr = " X: " + engine->ConvertIntToString((int)player->GetPosition().x) + " Y: " + engine->ConvertIntToString((int)player->GetPosition().y) + " Z: " + engine->ConvertIntToString((int)player->GetPosition().z);
				engine->RenderText(5, 135, "Player Position: " + playerPosStr);

				std::string playerRotStr = " X: " + engine->ConvertIntToString((int)player->GetRotation().x) + " Y: " + engine->ConvertIntToString((int)player->GetRotation().y) + " Z: " + engine->ConvertIntToString((int)player->GetRotation().z);
				engine->RenderText(5, 150, "Player Rotation: " + playerRotStr);

				engine->RenderText(5, 180, "Seed: " + engine->ConvertIntToString(engine->GetSeed()));
		}
}

void UpdatePhysics()
{
	
	player->UpdatePosition();
	world->UpdatePhysics(player);
	
}

int main(int, char**)
{
	engine = new XyEngine(Init, Render, Input, UpdatePhysics);
	world = new WorldManager(engine);

	engine->CreateWindow(1920, 1080, "SpaceChunks", 60.0f);

	delete player;
	delete world;
	delete engine;
	return 0;
}