#pragma once
#include "XyEngine.h"

class Camera
{
private:
	glm::vec3 m_CamPosition = glm::vec3(0.0f, 0.0f, 5.0f);
	glm::vec3 m_CamRotation = glm::vec3(0.0f, 0.0f, 0.0f);
	XyEngine* m_Engine;

	void LockCamera();
	void MoveCamera(float distance, float direction);
	void MoveCameraUp(float distance, float direction);

public:

	Camera(XyEngine* engine);

	glm::vec3 GetCameraPosition();
	void SetCameraPosition(glm::vec3 pos);

	glm::vec3 GetCameraRotation();
	void SetCameraRotation(glm::vec3 pos);

	void UpdateControls(float moveSpeed, float mouseSpeed, bool mouseIn);
	void UpdateCamera();
};


