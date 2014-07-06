#include "Camera.h"

Camera::Camera(XyEngine* engine)
{
	this->m_Engine = engine;
}

glm::vec3 Camera::GetCameraPosition()
{
	return m_CamPosition;
}

void Camera::SetCameraPosition(glm::vec3 pos)
{
	this->m_CamPosition = pos;
}

glm::vec3 Camera::GetCameraRotation()
{
	return m_CamRotation;
}

void Camera::SetCameraRotation(glm::vec3 pos)
{
	this->m_CamRotation = pos;
}

void Camera::LockCamera()
{
	if (m_CamRotation.y < 0.0)
		m_CamRotation.y += 360;

	if (m_CamRotation.y > 360.0)
		m_CamRotation.y -= 360;

	if (m_CamRotation.x > 90)
		m_CamRotation.x = 90;

	if (m_CamRotation.x < -90)
		m_CamRotation.x = -90;
}

void Camera::MoveCamera(float distance, float direction)
{
	float rad = (m_CamRotation.y + direction) * M_PI / 180.0;
	m_CamPosition.x -= sin(rad) * distance;
	m_CamPosition.z -= cos(rad) * distance;
}

void Camera::MoveCameraUp(float distance, float direction)
{
	float rad = (m_CamRotation.y + direction) * M_PI / 180.0;
	m_CamPosition.y += sin(rad)*distance;
}

void Camera::UpdateControls(float moveSpeed, float mouseSpeed, bool mouseIn)
{
	if (mouseIn)
	{
		SDL_ShowCursor(SDL_DISABLE);
		int tmpx, tmpy;
		SDL_GetMouseState(&tmpx, &tmpy); 
		m_CamRotation.y += mouseSpeed*(MidX - tmpx);
		m_CamRotation.x += mouseSpeed*(MidY - tmpy);
		LockCamera();

		const Uint8 *state = SDL_GetKeyboardState(NULL);

		if (state[SDL_SCANCODE_W])
		{
			MoveCamera(moveSpeed, 0.0);
		}
		else if (state[SDL_SCANCODE_S])
		{
			MoveCamera(moveSpeed, 180.0);
		}
		else if (state[SDL_SCANCODE_A])
		{
			MoveCamera(moveSpeed, 90.0);
		}
		else if (state[SDL_SCANCODE_D])
		{
			MoveCamera(moveSpeed, 270);
		}
		else if (state[SDL_SCANCODE_SPACE])
		{
			m_CamPosition.y += moveSpeed;
		}
		else if (state[SDL_SCANCODE_LSHIFT])
		{
			m_CamPosition.y -= moveSpeed;
		}

	}
	m_Engine->RotateWorldMatrix_X(-m_CamRotation.x);
	m_Engine->RotateWorldMatrix_Y(-m_CamRotation.y);
	//m_Engine->RotateWorldMatrix_Z(-m_CamRotation.z);
}

void Camera::UpdateCamera()
{
	m_Engine->TranslateWorldMatrix(-m_CamPosition);
}