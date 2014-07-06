#pragma once
#include "XyEngine.h"

class Frustum
{
public:
	float m_Frustum[6][4];

	enum sides
	{
		LEFT = 0,
		RIGHT,
		TOP,
		BOTTOM,
		FRONT,
		BACK
	};

	enum corners
	{
		A = 0,
		B,
		C,
		D,
	};

	static Frustum GetFrustum(XyEngine* engine)
	{
		static Frustum frustum;
		frustum.CalculateFrustum(engine);
		return frustum;
	}

	

	void NormalizePlane(float frustum[6][4], int side)
	{
		float magnitude = (float)sqrt(frustum[side][0] * frustum[side][0] + frustum[side][1] * frustum[side][1] + frustum[side][2] * frustum[side][2]);

		frustum[side][0] /= magnitude;
		frustum[side][1] /= magnitude;
		frustum[side][2] /= magnitude;
		frustum[side][3] /= magnitude;
	}
	void CalculateFrustum(XyEngine* engine)
	{
		glm::mat4 mat = engine->GetPipeline().GetModelViewProjectionMatrix();
		
		// Calculate the LEFT side (column1 + column4)
		m_Frustum[LEFT][A] = (mat[3][0]) + (mat[0][0]);
		m_Frustum[LEFT][B] = (mat[3][1]) + (mat[0][1]);
		m_Frustum[LEFT][C] = (mat[3][2]) + (mat[0][2]);
		m_Frustum[LEFT][D] = (mat[3][3]) + (mat[0][3]);

		// Calculate the RIGHT side (-column1 + column4)
		m_Frustum[RIGHT][A] = (mat[3][0]) - (mat[0][0]);
		m_Frustum[RIGHT][B] = (mat[3][1]) - (mat[0][1]);
		m_Frustum[RIGHT][C] = (mat[3][2]) - (mat[0][2]);
		m_Frustum[RIGHT][D] = (mat[3][3]) - (mat[0][3]);

		// Calculate the TOP side (-column2 + column4)
		m_Frustum[TOP][A] = (mat[3][0]) - (mat[1][0]);
		m_Frustum[TOP][B] = (mat[3][1]) - (mat[1][1]);
		m_Frustum[TOP][C] = (mat[3][2]) - (mat[1][2]);
		m_Frustum[TOP][D] = (mat[3][3]) - (mat[1][3]);

		// Calculate the BOTTOM side (column2 + column4)
		m_Frustum[BOTTOM][A] = (mat[3][0]) + (mat[1][0]);
		m_Frustum[BOTTOM][B] = (mat[3][1]) + (mat[1][1]);
		m_Frustum[BOTTOM][C] = (mat[3][2]) + (mat[1][2]);
		m_Frustum[BOTTOM][D] = (mat[3][3]) + (mat[1][3]);

		// Calculate the FRONT side (column3 + column4)
		m_Frustum[FRONT][A] = (mat[3][0]) + (mat[2][0]);
		m_Frustum[FRONT][B] = (mat[3][1]) + (mat[2][1]);
		m_Frustum[FRONT][C] = (mat[3][2]) + (mat[2][2]);
		m_Frustum[FRONT][D] = (mat[3][3]) + (mat[2][3]);

		// Calculate the BACK side (-column3 + column4)
		m_Frustum[BACK][A] = (mat[3][0]) - (mat[2][0]);
		m_Frustum[BACK][B] = (mat[3][1]) - (mat[2][1]);
		m_Frustum[BACK][C] = (mat[3][2]) - (mat[2][2]);
		m_Frustum[BACK][D] = (mat[3][3]) - (mat[2][3]);

		// Normalize all the sides
		NormalizePlane(m_Frustum, LEFT);
		NormalizePlane(m_Frustum, RIGHT);
		NormalizePlane(m_Frustum, TOP);
		NormalizePlane(m_Frustum, BOTTOM);
		NormalizePlane(m_Frustum, FRONT);
		NormalizePlane(m_Frustum, BACK);
	}

	 bool pointInFrustum(float x, float y, float z) {
		for (int i = 0; i < 6; i++) {
			if (m_Frustum[i][0] * x + m_Frustum[i][1] * y + m_Frustum[i][2] * z + m_Frustum[i][3] <= 0.0F) {
				return false;
			}
		}

		return true;
	}

	bool sphereInFrustum(float x, float y, float z, float radius) {
		for (int i = 0; i < 6; i++) {
			if (m_Frustum[i][0] * x + m_Frustum[i][1] * y + m_Frustum[i][2] * z + m_Frustum[i][3] <= -radius) {
				return false;
			}
		}

		return true;
	}

	bool cubeFullyInFrustum(float x1, float y1, float z1, float x2, float y2, float z2) {
		for (int i = 0; i < 6; i++) {
			if ( m_Frustum[i][0] * x1 +  m_Frustum[i][1] * y1 +  m_Frustum[i][2] * z1 +  m_Frustum[i][3] <= 0.0F)
				return false;
			if ( m_Frustum[i][0] * x2 +  m_Frustum[i][1] * y1 +  m_Frustum[i][2] * z1 +  m_Frustum[i][3] <= 0.0F)
				return false;
			if ( m_Frustum[i][0] * x1 +  m_Frustum[i][1] * y2 +  m_Frustum[i][2] * z1 +  m_Frustum[i][3] <= 0.0F)
				return false;
			if ( m_Frustum[i][0] * x2 +  m_Frustum[i][1] * y2 +  m_Frustum[i][2] * z1 +  m_Frustum[i][3] <= 0.0F)
				return false;
			if ( m_Frustum[i][0] * x1 +  m_Frustum[i][1] * y1 +  m_Frustum[i][2] * z2 +  m_Frustum[i][3] <= 0.0F)
				return false;
			if ( m_Frustum[i][0] * x2 +  m_Frustum[i][1] * y1 +  m_Frustum[i][2] * z2 +  m_Frustum[i][3] <= 0.0F)
				return false;
			if ( m_Frustum[i][0] * x1 +  m_Frustum[i][1] * y2 +  m_Frustum[i][2] * z2 +  m_Frustum[i][3] <= 0.0F)
				return false;
			if ( m_Frustum[i][0] * x2 +  m_Frustum[i][1] * y2 +  m_Frustum[i][2] * z2 +  m_Frustum[i][3] <= 0.0F)
				return false;
		}

		return true;
	}

	bool cubeInFrustum(float x1, float y1, float z1, float x2, float y2, float z2) 
	{
		for (int i = 0; i < 6; i++) 
		{
			if (( m_Frustum[i][0] * x1 +  m_Frustum[i][1] * y1 +  m_Frustum[i][2] * z1 +  m_Frustum[i][3] <= 0.0F) && 
				( m_Frustum[i][0] * x2 +  m_Frustum[i][1] * y1 +  m_Frustum[i][2] * z1 +  m_Frustum[i][3] <= 0.0F) && 
				( m_Frustum[i][0] * x1 +  m_Frustum[i][1] * y2 +  m_Frustum[i][2] * z1 +  m_Frustum[i][3] <= 0.0F) && 
				( m_Frustum[i][0] * x2 +  m_Frustum[i][1] * y2 +  m_Frustum[i][2] * z1 +  m_Frustum[i][3] <= 0.0F) && 
				( m_Frustum[i][0] * x1 +  m_Frustum[i][1] * y1 +  m_Frustum[i][2] * z2 +  m_Frustum[i][3] <= 0.0F) && 
				( m_Frustum[i][0] * x2 +  m_Frustum[i][1] * y1 +  m_Frustum[i][2] * z2 +  m_Frustum[i][3] <= 0.0F) && 
				( m_Frustum[i][0] * x1 +  m_Frustum[i][1] * y2 +  m_Frustum[i][2] * z2 +  m_Frustum[i][3] <= 0.0F) && 
				( m_Frustum[i][0] * x2 +  m_Frustum[i][1] * y2 +  m_Frustum[i][2] * z2 +  m_Frustum[i][3] <= 0.0F)) 
			{
				return false;
			}
		}
		return true;
	}
};