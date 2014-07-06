#pragma once
#include "XyEngine.h"

enum MatrixModes
{
	MODEL_MATRIX = 0,
	VIEW_MATRIX,
	PROJECTION_MATRIX
};

class Matrices
{
private:
	std::vector<glm::mat4> modelMatrix;
	std::vector<glm::mat4> viewMatrix;
	std::vector<glm::mat4> projectionMatrix;

	int currentMatrix;
	bool matricesReady;

	glm::mat4 modelViewMatrix;
	glm::mat4 modelViewProjectionMatrix;
	glm::mat3 normalMatrix;
public:
	Matrices()
	{
		modelMatrix.push_back(glm::mat4(1.0));
		viewMatrix.push_back(glm::mat4(1.0));
		projectionMatrix.push_back(glm::mat4(1.0));

		currentMatrix = MODEL_MATRIX;
		matricesReady = true;

		modelViewMatrix = glm::mat4(1.0);
		modelViewProjectionMatrix = glm::mat4(1.0);
		normalMatrix = glm::mat3(1.0);
	}

	void LoadIndentity()
	{
		if (currentMatrix == MODEL_MATRIX || currentMatrix == VIEW_MATRIX)
		{
			modelMatrix[modelMatrix.size() - 1] = glm::mat4(1.0);
			viewMatrix[viewMatrix.size() - 1] = glm::mat4(1.0);
		}
		else
		{
			projectionMatrix[projectionMatrix.size() - 1] = glm::mat4(1.0);
		}
		matricesReady = false;
	}

	void SetMatrixMode(int i)
	{
		switch (i)
		{
		case MODEL_MATRIX:
			currentMatrix = MODEL_MATRIX;
			break;
		case VIEW_MATRIX:
			currentMatrix = VIEW_MATRIX;
			break;
		case PROJECTION_MATRIX:
			currentMatrix = PROJECTION_MATRIX;
			break;
		default:
			break;
		}
		matricesReady = false;
	}

	void Translate(float x, float y, float z)
	{
		switch (currentMatrix)
		{
			case MODEL_MATRIX:
				modelMatrix[modelMatrix.size() - 1] *= glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
				break;
			case VIEW_MATRIX:
				viewMatrix[viewMatrix.size() - 1] *= glm::translate(glm::mat4(1.0f), glm::vec3(-x, -y, -z));
				break;		
			default:
				break;
		}
		matricesReady = false;
	}

	void Scale(float x, float y, float z)
	{
		switch (currentMatrix)
		{
		case MODEL_MATRIX:
			modelMatrix[modelMatrix.size() - 1] *= glm::scale(glm::mat4(1.0f), glm::vec3(x, y, z));
			break;
		case VIEW_MATRIX:
			viewMatrix[viewMatrix.size() - 1] *= glm::scale(glm::mat4(1.0f), glm::vec3(-x, -y, -z));
			break;
		default:
			break;
		}
		matricesReady = false;
	}

	void Scale(float i)
	{
		Scale(i, i, i);
	}

	void RotateX(float angle)
	{
		switch (currentMatrix)
		{
		case MODEL_MATRIX:
			modelMatrix[modelMatrix.size() - 1] *= glm::rotate(glm::mat4(1.0f), angle, glm::vec3(1.0f, 0.0f, 0.0f));
			break;
		case VIEW_MATRIX:
			viewMatrix[viewMatrix.size() - 1] *= glm::rotate(glm::mat4(1.0f), -angle, glm::vec3(1.0f, 0.0f, 0.0f));
			break;
		default:
			break;	
		}
		matricesReady = false;
	}

	void RotateY(float angle)
	{
		switch (currentMatrix)
		{
		case MODEL_MATRIX:
			modelMatrix[modelMatrix.size() - 1] *= glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f));
			break;
		case VIEW_MATRIX:
			viewMatrix[viewMatrix.size() - 1] *= glm::rotate(glm::mat4(1.0f), -angle, glm::vec3(0.0f, 1.0f, 0.0f));
			break;
		default:
			break;
		}
		matricesReady = false;
	}

	void RotateZ(float angle)
	{
		switch (currentMatrix)
		{
		case MODEL_MATRIX:
			modelMatrix[modelMatrix.size() - 1] *= glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 0.0f, 1.0f));
			break;
		case VIEW_MATRIX:
			viewMatrix[viewMatrix.size() - 1] *= glm::rotate(glm::mat4(1.0f), -angle, glm::vec3(0.0f, 0.0f, 1.0f));
			break;
		default:
			break;
		}
		matricesReady = false;
	}

	void SetPerspective(float angle, float aRatio, float near, float far)
	{
		projectionMatrix[projectionMatrix.size() - 1] = glm::perspective(angle, aRatio, near, far);
		matricesReady = false;
	}

	void SetOrtho(float left, float right, float top, float bottom, float near, float far)
	{
		projectionMatrix[projectionMatrix.size() - 1] = glm::ortho(left, right, top, bottom, near, far);
		matricesReady = false;
	}

	void PushMatrix()
	{
		glm::mat4 matrix;
		switch (currentMatrix)
		{
		case MODEL_MATRIX:
		{
			matrix = modelMatrix[modelMatrix.size() - 1];
			modelMatrix.push_back(matrix);
			break;
		}
		case VIEW_MATRIX:
		{
			matrix = viewMatrix[viewMatrix.size() - 1];
			viewMatrix.push_back(matrix);
			break;
		}
		case PROJECTION_MATRIX:
		{
			matrix = projectionMatrix[projectionMatrix.size() - 1];
			projectionMatrix.push_back(matrix);
			break;
		}
		}
		matricesReady = false;

	}

	void PopMatrix()
	{
		switch (currentMatrix)
		{
		case MODEL_MATRIX:
		{
			if (modelMatrix.size() > 1)
				modelMatrix.pop_back();
			break;
		}
		case VIEW_MATRIX:
		{
			if (viewMatrix.size() > 1)
				viewMatrix.pop_back();
			break;
		}
		case PROJECTION_MATRIX:
		{
			if (projectionMatrix.size() > 1)
				projectionMatrix.pop_back();
			break;
		}
		}
		matricesReady = false;
	}

	glm::mat4& GetModelMatrix()
	{
		return modelMatrix[modelMatrix.size() - 1];
	}

	glm::mat4& GetModelViewMatrix()
	{
		if (!matricesReady)
			modelViewMatrix = viewMatrix[viewMatrix.size() - 1] * modelMatrix[modelMatrix.size() - 1];
		return modelViewMatrix;
	}

	glm::mat4& GetModelViewProjectionMatrix()
	{
		if (!matricesReady)
			modelViewProjectionMatrix = projectionMatrix[projectionMatrix.size() - 1] * viewMatrix[viewMatrix.size() - 1] * modelMatrix[modelMatrix.size() - 1];
		return modelViewProjectionMatrix;
	}

	glm::mat4& GetProjectionMatrix()
	{
		return projectionMatrix[projectionMatrix.size() - 1];
	}

	glm::mat3& GetNormalMatrix()
	{
		if (!matricesReady)
			normalMatrix = glm::mat3(GetModelViewMatrix());
		return normalMatrix;
	}

	void UpdateMatrices(unsigned int programID)
	{
		if (!matricesReady)
		{
			modelViewMatrix = viewMatrix[viewMatrix.size() - 1] * modelMatrix[modelMatrix.size() - 1];
			modelViewProjectionMatrix = projectionMatrix[projectionMatrix.size() - 1] * viewMatrix[viewMatrix.size() - 1] * modelMatrix[modelMatrix.size() - 1];
			normalMatrix = glm::mat3(GetModelViewMatrix());
			matricesReady = true;
		}

		glUniformMatrix4fv(glGetUniformLocation(programID, "modelMatrix"), 1, GL_FALSE, &modelMatrix[modelMatrix.size() - 1][0][0]);
		glUniformMatrix4fv(glGetUniformLocation(programID, "viewMatrix"), 1, GL_FALSE, &viewMatrix[viewMatrix.size() - 1][0][0]);
		glUniformMatrix4fv(glGetUniformLocation(programID, "projectionMatrix"), 1, GL_FALSE, &projectionMatrix[projectionMatrix.size() - 1][0][0]);
		glUniformMatrix4fv(glGetUniformLocation(programID, "modelViewMatrix"), 1, GL_FALSE, &modelViewMatrix[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(programID, "mvp"), 1, GL_FALSE, &modelViewProjectionMatrix[0][0]);
		glUniformMatrix3fv(glGetUniformLocation(programID, "normalMatrix"), 1, GL_FALSE, &normalMatrix[0][0]);
	}
};