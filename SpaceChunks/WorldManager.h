#ifndef WORLD_MANAGER_H
#define WORLD_MANAGER_H

#include "XyEngine.h"
#include "Chunk.h"

#define WORLD_SMALL 12
#define WORLD_LARGE 24

const int SECLECTED_WORLD_SIZE = WORLD_LARGE;

class WorldManager
{
private:

	std::vector<Chunk*> m_ChunkList;

	glm::vec3 m_cameraPosition;
	glm::vec3 m_cameraView;

	int ASYNC_NUM_CHUNKS_PER_FRAME = 3;

public:

	WorldManager();

	void Update(glm::vec3 cameraPosition, glm::vec3 cameraView);

	void UpdateWorld(glm::vec3 cameraPosition, glm::vec3 cameraView);
	
	void UpdateGL();

	void UpdateSetupList();
	void UpdateRenderList();


	void UpdateRebuildList();
	void UpdateVisibilityList(glm::vec3 camPos);

	 
	Chunk* getChunkInWorld(int x, int y, int z)
	{
		Chunk* chunk = NULL;

		for (int i = 0; i < m_ChunkList.size(); i++)
		{
			Chunk *tmpChunk = m_ChunkList.at(i)->GetChunk(x, y, z);

			if (tmpChunk != NULL)
			{
				chunk = tmpChunk;
				continue;
			}
		}

		if (chunk != NULL)
			return chunk;

		return NULL;	
	}


	long getVerts()
	{
		long verts;

		verts = 0;

		for (int i = 0; i < m_ChunkList.size(); i++)
		{
			//verts = verts + m_ChunkList.at(i)->GetVerts();
		}

		return verts;
	}

	

	void DisposeWorld()
	{
		for (int i = 0; i < m_ChunkList.size(); i++)
		{
			m_ChunkList.at(i)->DisposeChunk();
		}
	}
};

#endif



