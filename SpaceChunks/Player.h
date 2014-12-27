#pragma once
#include "XyEngine.h"
#include "Camera.h"

class Player
{
public:
	Player(glm::vec3 pos, glm::vec3 rot, XyEngine* engine);

	void SetPosition(glm::vec3 pos);
	glm::vec3 GetPosition();
	void SetRotation(glm::vec3 rot);
	glm::vec3 GetRotation();

	void Update(bool mousein);
	void UpdatePosition();

	bool IsGrounded()
	{
		return m_Grounded;
	}

	void SetGrounded(bool g)
	{
		m_Grounded = g;
	}

private:
	glm::vec3 m_pos;
	XyEngine* m_engine;
	glm::vec3 m_rot;
	Camera* m_Camera;
	bool m_Grounded;
};


