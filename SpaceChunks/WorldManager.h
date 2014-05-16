#ifndef WORLD_MANAGER_H
#define WORLD_MANAGER_H

#include "XyEngine.h"
#include "Chunk.h"

#define WORLD_SMALL 12
#define WORLD_LARGE 18

const int SECLECTED_WORLD_SIZE = WORLD_LARGE;

class WorldManager
{
private:
	int m_chunksLoaded;
	Chunk *m_Chunks[SECLECTED_WORLD_SIZE][SECLECTED_WORLD_SIZE];
public:

	Chunk* getChunkInWorld(int x, int y, int z)
	{
		Chunk* chunk;

		for (int i = 0; i < SECLECTED_WORLD_SIZE; i++)
		{
			for (int j = 0; j < SECLECTED_WORLD_SIZE; j++)
			{
				chunk = m_Chunks[i][j]->GetChunk(x, y, z);

				if (chunk != NULL)
				{
					return chunk;
					break;
				}
			}
		}
				
	}

	void CreateWorld()
	{
		for (int x = 0; x < SECLECTED_WORLD_SIZE; x++)
		{
			for (int z = 0; z < SECLECTED_WORLD_SIZE; z++)
			{
				m_Chunks[x][z] = new Chunk(glm::vec3(x * CHUNK_X, 0, z * CHUNK_Z));
				m_Chunks[x][z]->CreateChunk();
			}
		}
		printf("World Created!");
	}

	void Update()
	{
		for (int x = 0; x < SECLECTED_WORLD_SIZE; x++)
		{
			for (int z = 0; z < SECLECTED_WORLD_SIZE; z++)
			{
				m_Chunks[x][z]->UpdateChunk();
			}
		}
	}

	void DisposeWorld()
	{
		for (int x = 0; x < SECLECTED_WORLD_SIZE; x++)
		{
			for (int z = 0; z < SECLECTED_WORLD_SIZE; z++)
			{
				m_Chunks[x][z]->DisposeChunk();
			}
		}
	}
};

#endif