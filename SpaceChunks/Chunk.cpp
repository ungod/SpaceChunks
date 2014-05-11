#include "Chunk.h"

unsigned int tex;

Chunk::Chunk(int x, int y, int z)
{
	m_x = x;
	m_y = y;
	m_z = z;

	memset(m_blocks, 0, sizeof m_blocks);
	m_disposed = false;
	
	m_changed = true;
	chunkID = glGenLists(1);
	tex = renderer->LoadTexture("img/dirt.jpg");
}

void Chunk::CreateChunk()
{
	for (int x = 0; x < CHUNK_X; x++)
	{
		for (int y = 0; y < CHUNK_Y; y++)
		{
			for (int z = 0; z < CHUNK_Z; z++)
			{
				m_blocks[x][y][z] = 1; //Dirt ID
			}
		}
	}

	RebuildChunk();
}

void Chunk::RebuildChunk()
{
	glNewList(chunkID, GL_COMPILE);

	glDisable(GL_LIGHTING);

	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, tex);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBegin(GL_QUADS);
	for (int x = 0; x < CHUNK_X; x++)
	{
		for (int y = 0; y < CHUNK_Y; y++)
		{
			for (int z = 0; z < CHUNK_Z; z++)
			{
				if (!IsBlockInView(x, y, z) && m_blocks[x][y][z] != 0 && m_blocks[x][y][z])
				{
					if (m_blocks[x][y][z] == 1)
					{
						difamb[0] = 0.0f;
						difamb[1] = 0.75f;
						difamb[2] = 0.0f;
						difamb[3] = 0.5f;
					}
					else if (m_blocks[x][y][z] == 2){
						difamb[0] = 0.25f;
						difamb[1] = 0.64f;
						difamb[2] = 0.91f;
						difamb[3] = 1.0f;
					}
					else {
						difamb[0] = 1.0f;
						difamb[1] = 1.0f;
						difamb[2] = 0.0f;
						difamb[3] = 1.0f;
					}

					//glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, difamb);
					//glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 32.0f);

					//Front Face
					if (!IsFaceZInView(x, y, z, false))
					{
						renderer->ImmediateNormal(0.0f, 0.0f, -1.0f);
						glTexCoord2f(1, 0);
						renderer->ImmediateVertex(x + 1, y, z);
						glTexCoord2f(0, 0);
						renderer->ImmediateVertex(x, y, z);
						glTexCoord2f(0, 1);
						renderer->ImmediateVertex(x, y + 1, z);
						glTexCoord2f(1, 1);
						renderer->ImmediateVertex(x + 1, y + 1, z);
					}

					// Back Face
					if (!IsFaceZInView(x, y, z, true))
					{
						renderer->ImmediateNormal(0.0f, 0.0f, 1.0f);
						glTexCoord2f(0, 0);
						renderer->ImmediateVertex(x, y, z + 1);
						glTexCoord2f(1, 0);
						renderer->ImmediateVertex(x + 1, y, z + 1);
						glTexCoord2f(1, 1);
						renderer->ImmediateVertex(x + 1, y + 1, z + 1);
						glTexCoord2f(0, 1);
						renderer->ImmediateVertex(x, y + 1, z + 1);
					}

					// Left Face
					if (!IsFaceXInView(x, y, z, true))
					{
						renderer->ImmediateNormal(1.0f, 0.0f, 0.0f);
						glTexCoord2f(0, 0);
						renderer->ImmediateVertex(x + 1, y, z + 1);
						glTexCoord2f(1, 0);
						renderer->ImmediateVertex(x + 1, y, z);
						glTexCoord2f(1, 1);
						renderer->ImmediateVertex(x + 1, y + 1, z);
						glTexCoord2f(0, 1);
						renderer->ImmediateVertex(x + 1, y + 1, z + 1);
					}

					// Right Face
					if (!IsFaceXInView(x, y, z, false))
					{
						renderer->ImmediateNormal(-1.0f, 0.0f, 0.0f);
						glTexCoord2f(0, 0);
						renderer->ImmediateVertex(x, y, z);
						glTexCoord2f(1, 0);
						renderer->ImmediateVertex(x, y, z + 1);
						glTexCoord2f(1, 1);
						renderer->ImmediateVertex(x, y + 1, z + 1);
						glTexCoord2f(0, 1);
						renderer->ImmediateVertex(x, y + 1, z);
					}

					// Bottom Face
					if (!IsFaceYInView(x, y, z, false))
					{
						renderer->ImmediateNormal(0.0f, -1.0f, 0.0f);
						glTexCoord2f(1, 1);
						renderer->ImmediateVertex(x, y, z);
						glTexCoord2f(0, 1);
						renderer->ImmediateVertex(x + 1, y, z);
						glTexCoord2f(0, 0);
						renderer->ImmediateVertex(x + 1, y, z + 1);
						glTexCoord2f(1, 0);
						renderer->ImmediateVertex(x, y, z + 1);
					}

					// Top Face
					if (!IsFaceYInView(x, y, z, true))
					{
						renderer->ImmediateNormal(0.0f, 1.0f, 0.0f);
						glTexCoord2f(1, 0);
						renderer->ImmediateVertex(x + 1, y + 1, z);
						glTexCoord2f(0, 0);
						renderer->ImmediateVertex(x, y + 1, z);
						glTexCoord2f(0, 1);
						renderer->ImmediateVertex(x, y + 1, z + 1);
						glTexCoord2f(1, 1);
						renderer->ImmediateVertex(x + 1, y + 1, z + 1);
					}

				}
				else
				{
					// Dont Render the Block
				}			
			}
		}
	}
	glEnd();

	glEnable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glEndList();
}

void Chunk::UpdateChunk()
{
	glEnable(GL_DEPTH_TEST);
	glTranslatef(m_x, m_y, m_z);
	glCallList(chunkID);

	if (m_changed)
	{
		m_changed = false;
		RebuildChunk();
	}
}

void Chunk::DisposeChunk()
{
	m_disposed = true;
	
	delete renderer;

	glDeleteLists(chunkID, 1);
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
IsBlockInView checks blocks around it, if all blocks around it are not air blocks, it will not render the block
*/
bool Chunk::IsBlockInView(int x, int y, int z)
{
	bool facesHidden[6];
	if (x > 0) {
		if (!m_blocks[x - 1][y][z] == 0) facesHidden[0] = true;
		else facesHidden[0] = false;
	}
	else {
		facesHidden[0] = false;
	}

	if (x < CHUNK_X - 1) {
		if (!m_blocks[x + 1][y][z] == 0) facesHidden[1] = true;
		else facesHidden[1] = false;
	}
	else {
		facesHidden[1] = false;
	}

	if (y > 0) {
		if (!m_blocks[x][y - 1][z] == 0) facesHidden[2] = true;
		else facesHidden[2] = false;
	}
	else {
		facesHidden[2] = false;
	}

	if (y < CHUNK_Y - 1) {
		if (!m_blocks[x][y + 1][z] == 0) facesHidden[3] = true;
		else facesHidden[3] = false;
	}
	else {
		facesHidden[3] = false;
	}

	if (z > 0) {
		if (!m_blocks[x][y][z - 1] == 0) facesHidden[4] = true;
		else facesHidden[4] = false;
	}
	else {
		facesHidden[4] = false;
	}

	if (z < CHUNK_Z - 1) {
		if (!m_blocks[x][y][z + 1]  == 0 ) facesHidden[5] = true;
		else facesHidden[5] = false;
	}
	else {
		facesHidden[5] = false;
	}
	return facesHidden[0] && facesHidden[1] && facesHidden[2] && facesHidden[3] && facesHidden[4] && facesHidden[5];
}

/*
IsFaceInView checks blocks around it, if all blocks around it are not air blocks, it will not render the faces next to a block
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
