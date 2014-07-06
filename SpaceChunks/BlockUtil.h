#pragma once
#include "XyEngine.h"

class BlockUtil
{
public:
	static uint32_t PackBlock(uint16_t r, uint16_t g, uint16_t b)
	{
		return (r << 16) + (g << 8) + (b);
	}

	static glm::vec3 UnpackBlock(uint32_t data)
	{
		uint16_t r = (data >> 16) & 0xff;
		uint16_t g = (data >> 8) & 0xff;
		uint16_t b = (data)& 0xff;

		return glm::vec3(r, g, b);
	}

	static glm::vec3 GetBaseRGBFromBlock(uint32_t block)
	{
		glm::vec3 i = UnpackBlock(block);

		i = (1.0f / 255)*i;

		return i;
	}

private:

};
	