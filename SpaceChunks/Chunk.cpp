#include "Chunk.h"

int seed;

GLuint cs;

float CalculateNoiseValue(glm::vec3 pos, glm::vec3 offset, float scale)
{

	float noiseX = abs((pos.x + offset.x) * scale);
	float noiseY = abs((pos.y + offset.y) * scale);
	float noiseZ = abs((pos.z + offset.z) * scale);

	return raw_noise_3d(noiseX, noiseY, noiseZ);
}

Chunk::Chunk(glm::vec3 pos, int id)
{
	m_position = pos;
	m_id = id;

	memset(m_blocks, 0, sizeof m_blocks);
	m_rendered = true;
	m_disposed = false;
	m_changed = true;
	m_list = glGenLists(1);

	seed = renderer->GetSeed();

	glm::vec3 grain0Offset(seed * 10000, seed * 10000, seed * 10000);
	glm::vec3 grain1Offset(seed * 10000, seed * 10000, seed * 10000);
	glm::vec3 grain2Offset(seed * 10000, seed * 10000, seed * 10000);

	for (int x = 0; x < CHUNK_X; x++)
	{
		for (int y = 0; y < CHUNK_Y; y++)
		{
			for (int z = 0; z < CHUNK_Z; z++)
			{
				glm::vec3 pos(x + m_position.x, y + m_position.y, z + m_position.z);

				float mountainValue = CalculateNoiseValue(pos, grain2Offset, 0.017f);
				mountainValue -= ((float)y / 28);

				float noiseValue = CalculateNoiseValue(pos, grain0Offset, 0.07f);

				noiseValue -= ((float)y / 16);
				noiseValue = std::max(noiseValue, CalculateNoiseValue(pos, grain1Offset, 0.04f));
				noiseValue -= ((float)y / 14);


				noiseValue -= mountainValue;
				noiseValue += (12 - (float)y) / 12;

				
				
				if (y < 6)
				{
					m_blocks[x][y][z] = Block::BLOCK_WATER;
				}
				else if (noiseValue > 0)
				{
					if (y < 8 && m_blocks[x][y][z] != Block::BLOCK_WATER)
					{
						m_blocks[x][y][z] = Block::BLOCK_SAND;
					}
					else
					{
						m_blocks[x][y][z] = Block::BLOCK_GRASS;
					}
				}
				else
				{
					//m_blocks[x][y][z].setBlockType(BlockType_Air);
				}
				
			}
		}
	}

	

}

void Chunk::InitGLCode()
{
	renderer->LoadShaderProgram(cs, "shaders/chunkShader");
}

void Chunk::RebuildChunk()
{
	m_changed = false;

	glNewList(m_list, GL_COMPILE);
	glBegin(GL_TRIANGLES);

	for (int x = 0; x < CHUNK_X; x++)
	{
		for (int y = 0; y < CHUNK_Y; y++)
		{
			for (int z = 0; z < CHUNK_Z; z++)
			{
				if (!m_blocks[x][y][z])
					continue;

				if (!IsFaceXInView(x, y, z, true))
				{
					glVertex3f(x, y, z);
					glVertex3f(x, y, z + 1);
					glVertex3f(x, y + 1, z);

					glVertex3f(x, y + 1, z);
					glVertex3f(x, y, z + 1);
					glVertex3f(x, y + 1, z + 1);
				}

				if (!IsFaceXInView(x, y, z, false))
				{
					glVertex3f(x + 1, y, z);
					glVertex3f(x + 1, y + 1, z);
					glVertex3f(x + 1, y, z + 1);

					glVertex3f(x + 1, y + 1, z);
					glVertex3f(x + 1, y + 1, z + 1);
					glVertex3f(x + 1, y, z + 1);
				}
			
				if (!IsFaceZInView(x, y, z, false))
				{
					glVertex3f(x, y, z + 1);
					glVertex3f(x + 1, y, z + 1);
					glVertex3f(x, y + 1, z + 1);
					glVertex3f(x, y + 1, z + 1);
					glVertex3f(x + 1, y, z + 1);
					glVertex3f(x + 1, y + 1, z + 1);

				}

				if (!IsFaceZInView(x, y, z, true))
				{
					glVertex3f(x, y, z + 1);
					glVertex3f(x + 1, y, z + 1);
					glVertex3f(x, y + 1, z + 1);
					glVertex3f(x, y + 1, z + 1);
					glVertex3f(x + 1, y, z + 1);
					glVertex3f(x + 1, y + 1, z + 1);
				}

				
				if (!IsFaceYInView(x, y, z, false))
				{
					glVertex3f(x, y, z);
					glVertex3f(x + 1, y, z);
					glVertex3f(x, y, z + 1);
					glVertex3f(x + 1, y, z);
					glVertex3f(x + 1, y, z + 1);
					glVertex3f(x, y, z + 1);
				}

				if (!IsFaceYInView(x, y, z, true))
				{
					glVertex3f(x, y + 1, z);
					glVertex3f(x, y + 1, z + 1);
					glVertex3f(x + 1, y + 1, z);
					glVertex3f(x + 1, y + 1, z);
					glVertex3f(x, y + 1, z + 1);
					glVertex3f(x + 1, y + 1, z + 1);
				}
				
			}
		}
	}

	glEnd();
	glEndList();
}


void Chunk::RenderChunk()
{		
	float x = m_position.x;
	float y = m_position.y;
	float z = m_position.z;

	if(m_changed)
		RebuildChunk();

	if (m_rendered)
	{
		glPushMatrix();
		renderer->TranslateWorldMatrix(x, y, z);
			
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);

		glCallList(m_list);

		glPopMatrix();		
	}
}

void Chunk::DisposeChunk()
{
	m_disposed = true;
	delete renderer;

	glDeleteLists(m_list, 1);

	renderer->DisposeShader(chunkShader);
}

Chunk* Chunk::GetChunk(int x, int y, int z)
{
	bool chunkCheck[6];

	float tx = m_position.x;
	float ty = m_position.y;
	float tz = m_position.z;

	float cx = CHUNK_X + tx;
	float cy = CHUNK_Y + ty;
	float cz = CHUNK_Z + tz;

	/*------------------------------*/

	if (x >= tx) // x greater than 0
	{
		chunkCheck[0] = true;
	}
	else
	{
		chunkCheck[0] = false;
	}

	if (x <= cx)  // x less that 16
	{
		chunkCheck[1] = true;
	}
	else{
		chunkCheck[1] = false;
	}

	/*------------------------------*/

	if (y >= ty)
	{
		chunkCheck[2] = true;
	}
	else
	{
		chunkCheck[2] = false;
	}

	if (y <= cy)
	{
		chunkCheck[3] = true;
	}
	else{
		chunkCheck[3] = false;
	}

	/*------------------------------*/

	if (z >= tz)
	{
		chunkCheck[4] = true;
	}
	else
	{
		chunkCheck[4] = false;
	}

	if (z <= cz)
	{
		chunkCheck[5] = true;
	}
	else{
		chunkCheck[5] = false;
	}

	if (chunkCheck[0] && chunkCheck[1] && chunkCheck[2] && chunkCheck[3] && chunkCheck[4] && chunkCheck[5])
	{
		printf("FoundChunk");
		return this;
	}
	return NULL;
}



uint8_t Chunk::getBlock(int x, int y, int z)
{
	return m_blocks[x][y][z];
}

void Chunk::setBlock(int x, int y, int z, uint8_t type)
{
	m_blocks[x][y][z] = type;
	m_changed = true;
}

Chunk::~Chunk()
{
	if (!m_disposed)
	{
		DisposeChunk();
	}
}

/*
IsFaceInView checks blocks around it, if all blocks around it are not air blocks, it will not render the block
*/
bool Chunk::IsFaceYInView(int x, int y, int z, bool reversed)
{
	bool faceHidden;

	if (reversed == false)
	{
		if (y > 0) {
			if (m_blocks[x][y - 1][z] == 0) faceHidden = false;
			else faceHidden = true;
		}
		else {
			faceHidden = showChunkFaces;
		}
		return faceHidden;
	}
	else {
		if (y < CHUNK_Y - 1) {
			if (m_blocks[x][y + 1][z] == 0) faceHidden = false;
			else faceHidden = true;
		}
		else {
			faceHidden = showChunkFaces;
		}
		return faceHidden;
	}
}

bool Chunk::IsFaceXInView(int x, int y, int z, bool reversed)
{
	bool faceHidden;

	if (reversed == false)
	{
		if (x > 0) {
			if (m_blocks[x - 1][y][z] == 0) faceHidden = false;
			else faceHidden = true;
		}
		else {
			faceHidden = showChunkFaces;
		}
		return faceHidden;
	}
	else {
		if (x < CHUNK_X - 1) {
			if (m_blocks[x + 1][y][z] == 0) faceHidden = false;
			else faceHidden = true;
		}
		else {
			faceHidden = showChunkFaces;
		}
		return faceHidden;
	}
}

bool Chunk::IsFaceZInView(int x, int y, int z, bool reversed)
{
	bool faceHidden;

	if (reversed == false)
	{
		if (z > 0) {
			if (m_blocks[x][y][z - 1] == 0) faceHidden = false;
			else faceHidden = true;
		}
		else {
			faceHidden = showChunkFaces;
		}
		return faceHidden;
	}
	else {
		if (z < CHUNK_Z - 1) {
			if (m_blocks[x][y][z + 1] == 0) faceHidden = false;
			else faceHidden = true;
		}
		else {
			faceHidden = showChunkFaces;
		}
		return faceHidden;
	}
}
