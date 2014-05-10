#include "XyEngine.h"

#define CHUNK_X 16
#define CHUNK_Y 16
#define CHUNK_Z 16

class Chunk
{
public:
	Chunk(glm::vec3 pos);
	void CreateChunk();
	void RebuildChunk();
	void DisposeChunk();
	virtual ~Chunk();

	void UpdateChunk();

	uint8_t getBlock(int x, int y, int z);
	void setBlock(int x, int y, int z, uint8_t type);

private:
	bool m_disposed;
	glm::vec3 m_position;
	uint8_t m_blocks[CHUNK_X][CHUNK_Y][CHUNK_Z];
	bool m_changed;

	GLuint m_vbo;

	bool IsBlockInView(int x, int y, int z);
	bool IsFaceXInView(int x, int y, int z, bool reversed);
	bool IsFaceYInView(int x, int y, int z, bool reversed);
	bool IsFaceZInView(int x, int y, int z, bool reversed);

	bool showChunkFaces = false;
	int chunkID;
	float difamb[4];

	XyEngine *renderer = new XyEngine();
};

