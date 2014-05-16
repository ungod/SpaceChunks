#ifndef BLOCK_H
#define BLOCK_H

enum BlockType
{
	BlockType_Air = 0,
	BlockType_Dirt,
	BlockType_Sand,
	BlockType_Stone,

	BlockType_Default = 1,

	BlockType_NumTypes,
};
class Block
{
public:
	BlockType getBlockType()
	{
		return m_blockType;
	}

	void setBlockType(BlockType type)
	{
		m_blockType = type;
	}

	bool isActive()
	{
		return m_active;
	}

	void setActive(bool active)
	{
		m_active = active;
	}

private:
	BlockType m_blockType;
	bool m_active;
	float m_lightLevel;
};
#endif
