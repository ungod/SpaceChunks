#ifndef CHUNK_H
#define CHUNK_H

#include "XyEngine.h"
#include "Block.h"
#include "Shader.h"

#define CHUNK_X 16
#define CHUNK_Y 32
#define CHUNK_Z 16


static int m_chunksLoaded;
class Chunk
{
public:
	Chunk(glm::vec3 pos);
	void CreateChunk();
	void RebuildChunk();
	void DisposeChunk();
	virtual ~Chunk();

	void setChunkRendered(bool value)
	{
		this->m_rendered = value;
	}

	bool isChunkRendered()
	{
		return m_rendered;
	}

	void UpdateChunk();
	Chunk* GetChunk(int x, int y, int z);

	BlockType getBlock(int x, int y, int z);
	void setBlock(int x, int y, int z, BlockType type);

	
private:
	void RenderLines();

	Shader* lineShader;

	bool m_disposed;
	glm::vec3 m_position;
	Block*** m_blocks;
	bool m_changed;
	bool m_rendered;

	bool IsBlockInView(int x, int y, int z);
	bool IsFaceXInView(int x, int y, int z, bool reversed);
	bool IsFaceYInView(int x, int y, int z, bool reversed);
	bool IsFaceZInView(int x, int y, int z, bool reversed);

	bool showChunkFaces = false;
	int chunkID, renderLineID, chunkX, chunkY, chunkZ;

	float difamb[4];

	GLint lineColor;
	GLuint RenderLineArray;
	GLuint RenderLineElementBuffer;
	GLuint RenderLineBuffer;

	XyEngine *renderer = new XyEngine();
};

#endif

