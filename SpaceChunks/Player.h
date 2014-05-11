#include "XyEngine.h"

class Player
{
public:
	Player(glm::vec3 pos, float pitch, float yaw);
	void SetPosition(glm::vec3 pos, float pitch, float yaw);
	glm::vec3 GetPos();
	void Update(bool mousein);
	void Init();

	void setPitch(float pitch)
	{
		this->m_pitch = pitch;
	}
	void setYaw(float yaw)
	{
		this->m_yaw = yaw;
	}

	float getYaw()
	{
		return m_yaw;
	}
	float getPitch()
	{
		return m_pitch;
	}

	~Player();
private:
	glm::vec3 m_pos;
	float m_yaw;
	float m_pitch;
};



