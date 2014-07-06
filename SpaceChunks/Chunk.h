#pragma once

#include "XyEngine.h"

typedef glm::detail::tvec4<GLbyte, glm::precision::mediump> byte4;

class Chunk
{
public:
	Chunk(glm::vec3 pos, XyEngine *engine);
	void Render();
	void Rebuild();

	Chunk *GetChunk(int x, int y, int z);
	glm::vec3 GetCenter();
	glm::vec3 GetPosition();

	uint32_t getBlock(int x, int y, int z);
	void setBlock(int x, int y, int z, uint32_t type);

	virtual ~Chunk();

	bool ShouldRebuildChunk()
	{
		return m_changed;
	}

	bool IsChunkLoaded()
	{
		return m_loaded;
	}

	void Unload()
	{
		
	}

private:	
	glm::vec3 m_position;
	bool m_loaded = false;
	uint32_t m_blocks[CHUNK_X][CHUNK_Y][CHUNK_Z];
	GLint m_attributeCoord;
	bool m_changed;
	XyEngine *renderer;
	GLuint vbo;
	int elements;
};
