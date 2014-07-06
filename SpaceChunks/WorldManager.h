#ifndef WORLD_MANAGER_H
#define WORLD_MANAGER_H

#include "XyEngine.h"
#include "Chunk.h"

#define WORLD_SMALL 12
#define WORLD_LARGE 30

#define NUM_RENDER_CHUNKS_PER_FRAME 5

const int SECLECTED_WORLD_SIZE = WORLD_LARGE;

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

public:

	WorldManager(XyEngine *renderer);
	void Update(glm::vec3 cameraPosition, glm::vec3 cameraView);

	void UpdateUnloadList();



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

	void SetBlock(int x, int y, int z);

	void Dispose();

	Chunk* getChunkInWorld(int x, int y, int z)
	{
		Chunk* chunk = NULL;

		for (int i = 0; i < m_ChunkList.size(); i++)
		{
			Chunk *tmpChunk = m_ChunkList.at(i)->GetChunk(x, y, z);

			if (tmpChunk != NULL)
			{
				chunk = tmpChunk;
				break;
			}
		}

		if (chunk != NULL)
			return chunk;

		return NULL;	
	}
};

#endif



