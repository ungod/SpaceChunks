#ifndef WORLD_MANAGER_H
#define WORLD_MANAGER_H

#include "XyEngine.h"
#include "Chunk.h"
#include "Block.h"
#include "Player.h"

#define WORLD_DEBUG_A 1
#define WORLD_DEBUG_B 4

#define WORLD_TINY 8
#define WORLD_SMALL 16
#define WORLD_LARGE 32
#define WORLD_HUGE 48

#define NUM_RENDER_CHUNKS_PER_FRAME 5

const int SECLECTED_WORLD_SIZE = WORLD_SMALL;

class Chunk;

class WorldManager
{
private:

	std::vector<Chunk*> m_ChunkList;

	std::vector<Chunk*> m_RebuildChunks;
	std::vector<Chunk*> m_CreateChunks;
	std::vector<Chunk*> m_RenderChunks;
	std::vector<Chunk*> m_UnloadChunks;

	XyEngine *m_renderer;

	glm::vec3 m_cameraPosition;
	glm::vec3 m_cameraView;

	int ASYNC_NUM_CHUNKS_PER_FRAME = 3;

	int chunksLoaded;
	int chunksFrustum;
	int viewDistance = 11;
	int totalFaces;
public:

	WorldManager(XyEngine *renderer);
	void Update(glm::vec3 cameraPosition, glm::vec3 cameraView);

	void UpdateUnloadList();

	std::vector<Chunk*> GetChunkList()
	{
		return m_ChunkList;
	}

	int GetTotalFaces()
	{

		return totalFaces;
	}

	void UpdateSetupList();
	void UpdateRenderList();

	int GetViewDistance()
	{
		return viewDistance;
	}

	int GetLoadedChunks()
	{
		return chunksLoaded;
	}
	int GetFrustumChunks()
	{
		return chunksFrustum;
	}

	void UpdateRebuildList();
	void UpdateVisibilityList(glm::vec3 camPos);

	void SetBlock(int x, int y, int z, BlockType type);
	BlockType GetBlock(int x, int y, int z);

	void UpdatePhysics(Player* player);

	void Dispose();

	Chunk* getChunkInWorld(int x, int y, int z)
	{

		return NULL;	
	}
};

#endif



