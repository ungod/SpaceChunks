#include "Player.h"
#include "Camera.h"

Player::Player(glm::vec3 pos, float pitch, float yaw)
{
	m_pos = pos;
	setCamX(m_pos.x);
	setCamY(m_pos.y);
	setCamZ(m_pos.z);

	setCamPitch(pitch);
	setCamYaw(yaw);
}

void Player::Init()
{
	//initskybox(100.0f);
}

void Player::SetPosition(glm::vec3 pos, float pitch, float yaw)
{
	m_pos = pos;
	setCamX(m_pos.x);
	setCamY(m_pos.y);
	setCamZ(m_pos.z);

	setCamPitch(pitch);
	setCamYaw(yaw);
}

glm::vec3 Player::GetPos()
{
	return m_pos;
}

void Player::Update(bool mousein)
{
	m_pos.x = getCamX();
	m_pos.y = getCamY();
	m_pos.z = getCamZ();

	m_pitch = getCamPitch();
	m_yaw = getcamYaw();

	setCamPitch(m_pitch);
	setCamYaw(m_yaw);
	setCamX(m_pos.x);
	setCamY(m_pos.y);
	setCamZ(m_pos.z);

	Control(0.3, 0.05, mousein);
	//drawSkybox();
	UpdateCamera();
}

Player::~Player()
{

}
