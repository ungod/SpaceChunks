#include "Player.h"
#include "Camera.h"
#include "SkyBox.h"

Player::Player(glm::vec3 pos)
{
	m_pos = pos;
	setCamX(m_pos.x);
	setCamY(m_pos.y);
	setCamZ(m_pos.z);
}

void Player::Init()
{
	initskybox(50.0f);
}

void Player::SetPosition(glm::vec3 pos)
{
	m_pos = pos;
	setCamX(m_pos.x);
	setCamY(m_pos.y);
	setCamZ(m_pos.z);
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

	setCamX(m_pos.x);
	setCamY(m_pos.y);
	setCamZ(m_pos.z);

	Control(0.3, 0.05, mousein);
	drawSkybox();
	UpdateCamera();
}

Player::~Player()
{
	killskybox();
}
