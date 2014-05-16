#include "Chunk.h"

unsigned int tex;

int seed;




Chunk::Chunk(glm::vec3 pos)
{
	m_position = pos;

	chunkX = m_position.x + CHUNK_X;
	chunkY = m_position.y + CHUNK_Y;
	chunkZ = m_position.z + CHUNK_Z;

	lineShader = new Shader("shaders/lineShader");
	tex = renderer->LoadTexture("img/dirt.jpg");
	GLfloat renderLineVBOData[] = { m_position.x, 0, m_position.z,
									m_position.x, chunkY, m_position.z,
	
									m_position.x, 0, m_position.z,
									chunkX, 0, m_position.z,

									m_position.x, chunkY, m_position.z,
									chunkX, chunkY, m_position.z,

									m_position.x, 0, chunkZ,
									chunkX, 0, chunkZ,

									m_position.x, chunkY, chunkZ,
									chunkX, chunkY, chunkZ,

									m_position.x, chunkY, m_position.z,
									m_position.x, chunkY, chunkZ,

									m_position.x, 0, m_position.z,
									m_position.x, 0, chunkZ
	};

	glGenVertexArrays(1, &RenderLineArray);
	glBindVertexArray(RenderLineArray);

	glGenBuffers(1, &RenderLineBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, RenderLineBuffer);

	glBufferData(GL_ARRAY_BUFFER, sizeof(renderLineVBOData), renderLineVBOData, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);

	/* Old Stuff */
	chunkID = glGenLists(1);

	m_disposed = false;
	
	m_blocks = new Block**[chunkX];

	for (int i = m_position.x; i < chunkX; i++)
	{
		m_blocks[i] = new Block*[chunkY];

		for (int j = m_position.y; j < chunkY; j++)
		{
			m_blocks[i][j] = new Block[chunkZ];
		}
	}

	m_changed = true;
	m_rendered = true;
}

float CalculateNoiseValue(glm::vec3 pos, glm::vec3 offset, float scale)
{
	float noiseX = abs((pos.x + offset.x) * scale);
	float noiseY = abs((pos.y + offset.y) * scale);
	float noiseZ = abs((pos.z + offset.z) * scale);

	return raw_noise_3d(noiseX, noiseY, noiseZ);
}

void Chunk::CreateChunk()
{
	seed = renderer->GetSeed();

	m_chunksLoaded++;

	glm::vec3 grain0Offset(seed * 10000, seed * 10000, seed * 10000);
	glm::vec3 grain1Offset(seed * 10000, seed * 10000, seed * 10000);
	glm::vec3 grain2Offset(seed * 10000, seed * 10000, seed * 10000);

	for (int x = m_position.x; x < chunkX; x++)
	{
		for (int y = m_position.y; y < chunkY; y++)
		{
			for (int z = m_position.z; z < chunkZ; z++)
			{			
				glm::vec3 pos(x, y, z);

				float mountainValue = CalculateNoiseValue(pos, grain2Offset, 0.012f);
				mountainValue -= ((float)y / 14);

				float noiseValue = CalculateNoiseValue(pos, grain0Offset, 0.03f);

				noiseValue -= ((float)y / 9);
				noiseValue = std::max(noiseValue, CalculateNoiseValue(pos, grain1Offset, 0.01f));
				noiseValue -= ((float)y / 7);


				noiseValue -= mountainValue;

				noiseValue += (6 - (float)y) / 6;

				if (noiseValue > 0)
				{				
					m_blocks[x][y][z].setBlockType(BlockType_Dirt);
					m_blocks[x][y][z].setActive(true);
				}
				else
				{
					m_blocks[x][y][z].setBlockType(BlockType_Air);
					m_blocks[x][y][z].setActive(false);
				}
			}
		}
	}
}

GLfloat AmbientLight[] = { 0.8, 0.8, 0.8, 0.8 };

void Chunk::RebuildChunk()
{
	glNewList(chunkID, GL_COMPILE);

	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBegin(GL_QUADS);
	for (int x = m_position.x; x < chunkX; x++)
	{
		for (int y = m_position.y; y < chunkY; y++)
		{
			for (int z = m_position.z; z < chunkZ; z++)
			{
				if (!IsBlockInView(x, y, z) && m_blocks[x][y][z].getBlockType() != BlockType_Air && m_blocks[x][y][z].isActive())
				{
					glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, AmbientLight);

					//Front Face
					if (!IsFaceZInView(x, y, z, false))
					{
						renderer->ImmediateNormal(0.0f, 0.0f, -1.0f);
						glTexCoord2f(1, 0);
						renderer->ImmediateVertex((GLfloat)x + 1, (GLfloat)y, (GLfloat)z);
						glTexCoord2f(0, 0);
						renderer->ImmediateVertex((GLfloat)x, (GLfloat)y, (GLfloat)z);
						glTexCoord2f(0, 1);
						renderer->ImmediateVertex((GLfloat)x, (GLfloat)y + 1, (GLfloat)z);
						glTexCoord2f(1, 1);
						renderer->ImmediateVertex((GLfloat)x + 1, (GLfloat)y + 1, (GLfloat)z);
					}

					// Back Face
					if (!IsFaceZInView(x, y, z, true))
					{
						renderer->ImmediateNormal(0.0f, 0.0f, 1.0f);
						glTexCoord2f(0, 0);
						renderer->ImmediateVertex((GLfloat)x, (GLfloat)y, (GLfloat)z + 1);
						glTexCoord2f(1, 0);
						renderer->ImmediateVertex((GLfloat)x + 1, (GLfloat)y, (GLfloat)z + 1);
						glTexCoord2f(1, 1);
						renderer->ImmediateVertex((GLfloat)x + 1, (GLfloat)y + 1, (GLfloat)z + 1);
						glTexCoord2f(0, 1);
						renderer->ImmediateVertex((GLfloat)x, (GLfloat)y + 1, (GLfloat)z + 1);
					}

					// Left Face
					if (!IsFaceXInView(x, y, z, true))
					{
						renderer->ImmediateNormal(1.0f, 0.0f, 0.0f);
						glTexCoord2f(0, 0);
						renderer->ImmediateVertex((GLfloat)x + 1, (GLfloat)y, (GLfloat)z + 1);
						glTexCoord2f(1, 0);
						renderer->ImmediateVertex((GLfloat)x + 1, (GLfloat)y, (GLfloat)z);
						glTexCoord2f(1, 1);
						renderer->ImmediateVertex((GLfloat)x + 1, (GLfloat)y + 1, (GLfloat)z);
						glTexCoord2f(0, 1);
						renderer->ImmediateVertex((GLfloat)x + 1, (GLfloat)y + 1, (GLfloat)z + 1);
					}

					// Right Face
					if (!IsFaceXInView(x, y, z, false))
					{
						renderer->ImmediateNormal(-1.0f, 0.0f, 0.0f);
						glTexCoord2f(0, 0);
						renderer->ImmediateVertex((GLfloat)x, (GLfloat)y, (GLfloat)z);
						glTexCoord2f(1, 0);
						renderer->ImmediateVertex((GLfloat)x, (GLfloat)y, (GLfloat)z + 1);
						glTexCoord2f(1, 1);
						renderer->ImmediateVertex((GLfloat)x, (GLfloat)y + 1, (GLfloat)z + 1);
						glTexCoord2f(0, 1);
						renderer->ImmediateVertex((GLfloat)x, (GLfloat)y + 1, (GLfloat)z);
					}

					// Bottom Face
					if (!IsFaceYInView(x, y, z, false))
					{
						renderer->ImmediateNormal(0.0f, -1.0f, 0.0f);
						glTexCoord2f(1, 1);
						renderer->ImmediateVertex((GLfloat)x, (GLfloat)y, (GLfloat)z);
						glTexCoord2f(0, 1);
						renderer->ImmediateVertex((GLfloat)x + 1, (GLfloat)y, (GLfloat)z);
						glTexCoord2f(0, 0);
						renderer->ImmediateVertex((GLfloat)x + 1, (GLfloat)y, (GLfloat)z + 1);
						glTexCoord2f(1, 0);
						renderer->ImmediateVertex((GLfloat)x, (GLfloat)y, (GLfloat)z + 1);
					}

					// Top Face
					if (!IsFaceYInView(x, y, z, true))
					{
						renderer->ImmediateNormal(0.0f, 1.0f, 0.0f);
						glTexCoord2f(1, 0);
						renderer->ImmediateVertex((GLfloat)x + 1, (GLfloat)y + 1, (GLfloat)z);
						glTexCoord2f(0, 0);
						renderer->ImmediateVertex((GLfloat)x, (GLfloat)y + 1, (GLfloat)z);
						glTexCoord2f(0, 1);
						renderer->ImmediateVertex((GLfloat)x, (GLfloat)y + 1, (GLfloat)z + 1);
						glTexCoord2f(1, 1);
						renderer->ImmediateVertex((GLfloat)x + 1, (GLfloat)y + 1, (GLfloat)z + 1);
					}
				}		
			}
		}
	}
	
	glEnd();	
	glEndList();
}

void Chunk::UpdateChunk()
{
	glEnable(GL_DEPTH_TEST);

	if (m_changed)
	{		
		RebuildChunk();
		m_changed = false;
	}

	if (m_rendered)
	{
		glBindTexture(GL_TEXTURE_2D, tex);
		glCallList(chunkID);
	}
		
	if (ShowGrid)
	{
		glDisable(GL_TEXTURE_2D);

		lineShader->Bind();

		glBindVertexArray(RenderLineArray);
		glDrawArrays(GL_LINES, 0, 14);

		lineShader->UnBind();

		glEnable(GL_TEXTURE_2D);
	}
}

Chunk* Chunk::GetChunk(int x, int y, int z)
{
	bool chunkCheck[6];

	/*------------------------------*/

	if (x >= m_position.x) // x greater than 0
	{
		chunkCheck[0] = true;
	}
	else
	{
		chunkCheck[0] = false;
	}

	if (x <= chunkX)  // x less that 16
	{
		chunkCheck[1] = true;
	}
	else{
		chunkCheck[1] = false;
	}

	/*------------------------------*/

	if (y >= m_position.y)
	{
		chunkCheck[2] = true;
	}
	else
	{
		chunkCheck[2] = false;
	}

	if (y <= chunkY)
	{
		chunkCheck[3] = true;
	}
	else{
		chunkCheck[3] = false;
	}

	/*------------------------------*/

	if (z >= m_position.z)
	{
		chunkCheck[4] = true;
	}
	else
	{
		chunkCheck[4] = false;
	}

	if (z <= chunkZ)
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
	return NULL;
}

void Chunk::DisposeChunk()
{
	m_disposed = true;
	
	glDeleteVertexArrays(1, &RenderLineArray);

	delete renderer;

	glDeleteLists(chunkID, 1);

	for (int i = m_position.x; i < chunkX; i++)
	{
		for (int j = m_position.y; j < chunkY; j++)
		{
			delete[] m_blocks[i][j];
		}

		delete[] m_blocks[i];
	}

	delete[] m_blocks;
}

BlockType Chunk::getBlock(int x, int y, int z)
{
	return m_blocks[x][y][z].getBlockType();
}

void Chunk::setBlock(int x, int y, int z, BlockType type)
{
	m_blocks[x][y][z].setBlockType(type);
	m_blocks[x][y][z].setActive(true);
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
IsBlockInView checks blocks around it, if all blocks around it are not air blocks, it will not render the block
*/
bool Chunk::IsBlockInView(int x, int y, int z)
{
	bool facesHidden[6];

	if (x > m_position.x) {
		if (!m_blocks[x - 1][y][z].getBlockType() == BlockType_Air) facesHidden[0] = true;
		else facesHidden[0] = false;
	}
	else {
		facesHidden[0] = false;
	}

	if (x < chunkX - 1) {
		if (!m_blocks[x + 1][y][z].getBlockType() == BlockType_Air) facesHidden[1] = true;
		else facesHidden[1] = false;
	}
	else {
		facesHidden[1] = false;
	}

	if (y > m_position.y) {
		if (!m_blocks[x][y - 1][z].getBlockType() == BlockType_Air) facesHidden[2] = true;
		else facesHidden[2] = false;
	}
	else {
		facesHidden[2] = false;
	}

	if (y < chunkY - 1) {
		if (!m_blocks[x][y + 1][z].getBlockType() == BlockType_Air) facesHidden[3] = true;
		else facesHidden[3] = false;
	}
	else {
		facesHidden[3] = false;
	}

	if (z > m_position.z) {
		if (!m_blocks[x][y][z - 1].getBlockType() == BlockType_Air) facesHidden[4] = true;
		else facesHidden[4] = false;
	}
	else {
		facesHidden[4] = false;
	}

	if (z < chunkZ - 1) {
		if (!m_blocks[x][y][z + 1].getBlockType() == BlockType_Air) facesHidden[5] = true;
		else facesHidden[5] = false;
	}
	else {
		facesHidden[5] = false;
	}
	return facesHidden[0] && facesHidden[1] && facesHidden[2] && facesHidden[3] && facesHidden[4] && facesHidden[5];
}

/*
IsFaceInView checks blocks around it, if all blocks around it are not air blocks, it will not render the block
*/
bool Chunk::IsFaceYInView(int x, int y, int z, bool reversed)
{
	bool faceHidden;


	if (reversed == false)
	{
		if (y > m_position.y) {
			if (m_blocks[x][y - 1][z].getBlockType() == BlockType_Air) faceHidden = false;
			else faceHidden = true;
		}
		else {
			faceHidden = showChunkFaces;
		}
		return faceHidden;
	}
	else {
		if (y < chunkY - 1) {
			if (m_blocks[x][y + 1][z].getBlockType() == BlockType_Air) faceHidden = false;
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
		if (x > m_position.x) {
			if (m_blocks[x - 1][y][z].getBlockType() == BlockType_Air) faceHidden = false;
			else faceHidden = true;
		}
		else {
			faceHidden = showChunkFaces;
		}
		return faceHidden;
	}
	else {
		if (x < chunkX - 1) {
			if (m_blocks[x + 1][y][z].getBlockType() == BlockType_Air) faceHidden = false;
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
		if (z > m_position.z) {
			if (m_blocks[x][y][z - 1].getBlockType() == BlockType_Air) faceHidden = false;
			else faceHidden = true;
		}
		else {
			faceHidden = showChunkFaces;
		}
		return faceHidden;
	}
	else {
		if (z < chunkZ - 1) {
			if (m_blocks[x][y][z + 1].getBlockType() == BlockType_Air) faceHidden = false;
			else faceHidden = true;
		}
		else {
			faceHidden = showChunkFaces;
		}
		return faceHidden;
	}
}
