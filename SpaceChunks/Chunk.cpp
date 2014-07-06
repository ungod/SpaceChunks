#include "Chunk.h"

#include <GLM/gtc/noise.hpp>

#include "BlockUtil.h"

int seed;
float CalculateNoiseValue(glm::vec3 pos, glm::vec3 offset, float scale)
{

	float noiseX = abs((pos.x + offset.x) * scale);
	float noiseY = abs((pos.y + offset.y) * scale);
	float noiseZ = abs((pos.z + offset.z) * scale);

	return std::max(0.0f, glm::simplex(glm::vec2(noiseX, noiseZ)));
}

int Noise(int x, int y, float scale, float mag, float exp)
{
	return (int)(pow(glm::simplex(glm::vec2(x / scale, y / scale)*mag), exp));
}

Chunk::Chunk(glm::vec3 pos, XyEngine *engine)
{
	m_position = pos;
	renderer = engine;

	memset(m_blocks, 0, sizeof m_blocks);
	m_changed = true;
	glGenBuffers(1, &vbo);

	seed = renderer->GetSeed();

	glm::vec3 grain0Offset(seed * 10000, seed * 10000, seed * 10000);
	glm::vec3 grain1Offset(seed * 10000, seed * 10000, seed * 10000);
	glm::vec3 grain2Offset(seed * 10000, seed * 10000, seed * 10000);

	for (int x = 0; x < CHUNK_X; x++)
	{
		float noiseX = ((float)x + m_position.x) / 20;

		for (int y = 0; y < CHUNK_Y; y++)
		{
			float noiseY = ((float)y + m_position.y) / 35;

			for (int z = 0; z < CHUNK_Z; z++)
			{
				float noiseZ = ((float)z + m_position.z) / 20;

				float noiseValue = glm::simplex(glm::vec3(noiseX, noiseY, noiseZ));

				noiseValue += (20.0f - (float)y) / 10;

				if (noiseValue > 0.2f)
				{
					m_blocks[x][y][z] = BlockUtil::PackBlock(255, 0, 0);
				}
					
			}
		}
	}

	m_loaded = true;
}

glm::vec3 Chunk::GetCenter()
{
	return glm::vec3(m_position.x - (CHUNK_X / 2), m_position.y - (CHUNK_Y / 2), m_position.z - (CHUNK_Z / 2));
}

void Chunk::Rebuild()
{
	m_changed = false;

	byte4 vertex[CHUNK_X * CHUNK_Y * CHUNK_Z * 18];
	int i = 0;
	int faces = 0;

	for (int x = 0; x < CHUNK_X; x++)
	{
		for (int y = 0; y < CHUNK_Y; y++)
		{
			for (int z = 0; z < CHUNK_Z; z++)
			{
				if (!m_blocks[x][y][z])
					continue;

				//glm::vec3 col = BlockUtil::GetBaseRGBFromBlock(m_blocks[x][y][z]);
				//std::cout << col.r << " " << col.g << " " << col.b << " " << std::endl;

				if (!renderer->IsFaceXInView(x, y, z, false, m_blocks))
				{
					vertex[i++] = byte4(x, y, z, m_blocks[x][y][z]);
					vertex[i++] = byte4(x, y, z + 1, m_blocks[x][y][z]);
					vertex[i++] = byte4(x, y + 1, z, m_blocks[x][y][z]);
					vertex[i++] = byte4(x, y + 1, z, m_blocks[x][y][z]);
					vertex[i++] = byte4(x, y, z + 1, m_blocks[x][y][z]);
					vertex[i++] = byte4(x, y + 1, z + 1, m_blocks[x][y][z]);
					faces++;
				}

				if (!renderer->IsFaceXInView(x, y, z, true, m_blocks))
				{
					vertex[i++] = byte4(x + 1, y, z, m_blocks[x][y][z]);
					vertex[i++] = byte4(x + 1, y + 1, z, m_blocks[x][y][z]);
					vertex[i++] = byte4(x + 1, y, z + 1, m_blocks[x][y][z]);
					vertex[i++] = byte4(x + 1, y + 1, z, m_blocks[x][y][z]);
					vertex[i++] = byte4(x + 1, y + 1, z + 1, m_blocks[x][y][z]);
					vertex[i++] = byte4(x + 1, y, z + 1, m_blocks[x][y][z]);
					faces++;
				}
				
				if (!renderer->IsFaceYInView(x, y, z, false, m_blocks))
				{
					vertex[i++] = byte4(x, y, z, -1);
					vertex[i++] = byte4(x + 1, y, z, -1);
					vertex[i++] = byte4(x, y, z + 1, -1);
					vertex[i++] = byte4(x + 1, y, z, -1);
					vertex[i++] = byte4(x + 1, y, z + 1, -1);
					vertex[i++] = byte4(x, y, z + 1, -1);
					faces++;
				}

				if (!renderer->IsFaceYInView(x, y, z, true, m_blocks))
				{
					vertex[i++] = byte4(x, y + 1, z, -1);
					vertex[i++] = byte4(x, y + 1, z + 1, -1);
					vertex[i++] = byte4(x + 1, y + 1, z, -1);
					vertex[i++] = byte4(x + 1, y + 1, z, -1);
					vertex[i++] = byte4(x, y + 1, z + 1, -1);
					vertex[i++] = byte4(x + 1, y + 1, z + 1, -1);
					faces++;
				}
				
				if (!renderer->IsFaceZInView(x, y, z, false, m_blocks))
				{
					vertex[i++] = byte4(x, y, z, m_blocks[x][y][z]);
					vertex[i++] = byte4(x, y + 1, z, m_blocks[x][y][z]);
					vertex[i++] = byte4(x + 1, y, z, m_blocks[x][y][z]);
					vertex[i++] = byte4(x, y + 1, z, m_blocks[x][y][z]);
					vertex[i++] = byte4(x + 1, y + 1, z, m_blocks[x][y][z]);
					vertex[i++] = byte4(x + 1, y, z, m_blocks[x][y][z]);
					faces++;
				}

				if (!renderer->IsFaceZInView(x, y, z, true, m_blocks))
				{
					vertex[i++] = byte4(x, y, z + 1, m_blocks[x][y][z]);
					vertex[i++] = byte4(x + 1, y, z + 1, m_blocks[x][y][z]);
					vertex[i++] = byte4(x, y + 1, z + 1, m_blocks[x][y][z]);
					vertex[i++] = byte4(x, y + 1, z + 1, m_blocks[x][y][z]);
					vertex[i++] = byte4(x + 1, y, z + 1, m_blocks[x][y][z]);
					vertex[i++] = byte4(x + 1, y + 1, z + 1, m_blocks[x][y][z]);
					faces++;
				}
				
			}
		}
	}

	elements = i;
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, i * sizeof *vertex, vertex, GL_STATIC_DRAW);
}

glm::vec3 Chunk::GetPosition()
{
	return m_position;
}

void Chunk::Render()
{		
	renderer->PushMatrix();

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	renderer->AddUniformVector4("colorIn", glm::vec4(0.0f, 0.41f, 0.23f, 1.0f));

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 4, GL_BYTE, GL_FALSE, 0, 0);
	renderer->TranslateWorldMatrix(m_position);
	renderer->UpdateMatrix();
	glDrawArrays(GL_TRIANGLES, 0, elements);
	
	glDisableVertexAttribArray(0);

	renderer->PopMatrix();
}

Chunk* Chunk::GetChunk(int x, int y, int z)
{
	bool chunkCheck[6];

	float x1 = GetPosition().x;
	float y1 = GetPosition().y;
	float z1 = GetPosition().z;

	float x2 = GetPosition().x + CHUNK_X;
	float y2 = GetPosition().y + CHUNK_Y;
	float z2 = GetPosition().z + CHUNK_Z;

	if (x >= x1)
	{
		chunkCheck[0] = true;
	}
	else
	{
		chunkCheck[0] = false;
	}

	if (x <= x2)
	{
		chunkCheck[1] = true;
	}
	else{
		chunkCheck[1] = false;
	}

	if (y >= y1)
	{
		chunkCheck[2] = true;
	}
	else
	{
		chunkCheck[2] = false;
	}

	if (y <= y2)
	{
		chunkCheck[3] = true;
	}
	else{
		chunkCheck[3] = false;
	}

	if (z >= z1)
	{
		chunkCheck[4] = true;
	}
	else
	{
		chunkCheck[4] = false;
	}

	if (z <= z2)
	{
		chunkCheck[5] = true;
	}
	else{
		chunkCheck[5] = false;
	}

	if (chunkCheck[0] && chunkCheck[1] && chunkCheck[2] && chunkCheck[3] && chunkCheck[4] && chunkCheck[5])
	{
		return this;
	}
	else
	{
		return NULL;
	}
}

uint32_t Chunk::getBlock(int x, int y, int z)
{
	return m_blocks[x][y][z];
}

void Chunk::setBlock(int x, int y, int z, uint32_t type)
{
	m_blocks[x][y][z] = type;
	m_changed = true;
}

Chunk::~Chunk()
{
	m_loaded = false;
	glDeleteBuffers(1, &vbo);
	delete m_blocks;
}