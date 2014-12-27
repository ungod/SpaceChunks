#pragma once

enum BlockType
{
	Air = 0,

	Grass,
	Dirt,

	NumTypes,
};

class Block
{
public:
	Block();
	~Block();

	bool IsActive();
	BlockType GetBlockType();

	void SetActive(bool active);
	void SetBlockType(BlockType type);

private:
	bool m_pActive;
	BlockType m_pBlockType;
};

