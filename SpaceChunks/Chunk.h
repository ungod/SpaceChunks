#ifndef CHUNK_H
#define CHUNK_H

#include "XyEngine.h"
#include "Block.h"
#include "Shader.h"

#define CHUNK_X 16
#define CHUNK_Y 16
#define CHUNK_Z 16

class Chunk
{
public:
	Chunk(glm::vec3 pos, int id);

	typedef glm::detail::tvec4<GLbyte, glm::precision::mediump> byte4;

	void RenderChunk();
	void RebuildChunk();
	void DisposeChunk();


	void InitGLCode();

	Chunk *GetChunk(int x, int y, int z);
	uint8_t getBlock(int x, int y, int z);
	void setBlock(int x, int y, int z, uint8_t type);

	GLuint chunkShader;
	GLuint chunkTexture;

	virtual ~Chunk();

	void setChunkRendered(bool value)
	{
		this->m_rendered = value;
	}

	bool isChunkRendered()
	{
		return m_rendered;
	}

	void setChanged(bool value)
	{
		m_changed = value;
	}

	bool ShouldRebuildChunk()
	{
		return m_changed;
	}

	
private:	
	glm::vec3 m_position;
	uint8_t m_blocks[CHUNK_X][CHUNK_Y][CHUNK_Z];
	GLuint m_list;
	bool m_changed;
	bool m_disposed;	
	bool m_rendered;

	int m_id;

	bool IsBlockInView(int x, int y, int z);
	bool IsFaceXInView(int x, int y, int z, bool reversed);
	bool IsFaceYInView(int x, int y, int z, bool reversed);
	bool IsFaceZInView(int x, int y, int z, bool reversed);

	bool showChunkFaces = false;

	XyEngine *renderer = new XyEngine();
};

#endif

