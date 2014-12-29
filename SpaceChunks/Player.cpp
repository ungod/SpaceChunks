#include "Player.h"

Player::Player(glm::vec3 pos, glm::vec3 rot, XyEngine* engine)
{
	this->m_engine = engine;
	this->m_pos = pos;
	this->m_rot = rot;
	this->m_Camera = new Camera(engine);
	
	this->m_Camera->SetCameraPosition(this->m_pos);
	this->m_Camera->SetCameraPosition(this->m_rot);
}

void Player::SetPosition(glm::vec3 pos)
{
	this->m_pos = pos;
	this->m_Camera->SetCameraPosition(this->m_pos);
}

void Player::SetRotation(glm::vec3 rot)
{
	this->m_rot = rot;
	this->m_Camera->SetCameraPosition(this->m_rot);
}

glm::vec3 Player::GetPosition()
{
	return this->m_pos;
}

glm::vec3 Player::GetRotation()
{
	return this->m_rot;
}

void Player::UpdatePosition()
{
	this->m_pos = this->m_Camera->GetCameraPosition();
	this->m_rot = this->m_Camera->GetCameraRotation();
}

void Player::Update(bool mousein)
{
	this->m_Camera->SetCameraPosition(this->m_pos);
	this->m_Camera->SetCameraRotation(this->m_rot);

	this->m_Camera->UpdateControls(0.2f, 0.05f, mousein);
	this->m_Camera->UpdateCamera();
}
