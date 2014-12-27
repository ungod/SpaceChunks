#pragma once

#include "XyEngine.h"
#include "WorldManager.h"
#include "Block.h"

class WorldManager;

class Chunk
{
public:
	Chunk(glm::vec3 pos, XyEngine *engine, WorldManager *world);
	~Chunk();

	void Render();
	void Rebuild();

	glm::vec3 GetCenter();
	glm::vec3 GetPosition();
	Chunk* FindChunk(glm::vec3 pos);

	static BlockType GetTheoreticalBlock(glm::vec3 pos, glm::vec3 offset0, glm::vec3 offset1, glm::vec3 offset2);
	static BlockType GetTheoreticalBlock(glm::vec3 pos);

	BlockType GetBlock(int x, int y, int z);
	BlockType GetBlock(glm::vec3 pos);

	void SetBlock(int x, int y, int z, BlockType type);
	
	static float CalculateNoiseValue(glm::vec3 pos, glm::vec3 offset, float scale);
	bool IsTransparent(int x, int y, int z);

	bool ShouldRebuildChunk();
	bool IsChunkLoaded();

	Chunk* GetChunk(int x, int y, int z);

	Block*** GetBlockArray()
	{
		return m_pBlocks;
	}

private:	
	glm::vec3 m_position;
	Block*** m_pBlocks;
	bool m_pChanged;
	bool m_pLoaded = false;

	static XyEngine *m_pRenderer;
	static WorldManager* m_pWorld;

	GLuint m_pChunkID;
	GLuint m_pTextureID;
};
