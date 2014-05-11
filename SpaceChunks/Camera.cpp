#include "Camera.h"
#include "XyEngine.h"

float camX = 0.0, camY = 0.0, camZ = 5.0;
float camYaw = 0.0;
float camPitch = 0.0;

#ifndef M_PI
#define M_PI = 3.14159265358979
#endif

float getcamYaw()
{
	return camYaw;
}

float getCamPitch()
{
	return camPitch;
}

void setCamPitch(float pitch)
{
	camPitch = pitch;
}

void setCamYaw(float yaw)
{
	camYaw = yaw;
}

float getCamX()
{
	return camX;
}
float getCamY()
{
	return camY;

}
float getCamZ()
{
	return camZ;
}

void setCamX(float x)
{
	camX = x;
}
void setCamY(float y)
{
	camY = y;
}
void setCamZ(float z){
	camZ = z;
}

void lockCamera()
{
	if (camPitch > 90)
		camPitch = 90;

	if (camPitch < -90)
		camPitch = -90;

	if (camYaw < 0.0)
		camYaw += 360;

	if (camYaw > 360.0)
		camYaw -= 360;
}

void moveCamera(float dist, float dir)
{
	float rad = (camYaw + dir)*M_PI / 180.0;      //convert the degrees into radians
	camX -= sin(rad)*dist;    //calculate the new coorinate, if you don't understand, draw a right triangle with the datas, you have
	camZ -= cos(rad)*dist;    //and try to calculate the new coorinate with trigonometric functions, that should help
}

void moveCameraUp(float dist, float dir)
{
	//the the same, only this time we calculate the y coorinate
	float rad = (camPitch + dir)*M_PI / 180.0;
	camY += sin(rad)*dist;
}

void Control(float movevel, float mousevel, bool mi)      //move and mouse sensitivity, and is the mouse inside the window?
{
	if (mi)  //if the mouse is in the screen
	{
		SDL_ShowCursor(SDL_DISABLE);    //we don't show the cursor
		int tmpx, tmpy;
		SDL_GetMouseState(&tmpx, &tmpy); //get the current position of the cursor
		camYaw += mousevel*(MidX - tmpx);   //get the rotation, for example, if the mouse current position is 315, than 5*0.2, this is for Y
		camPitch += mousevel*(MidY - tmpy); //this is for X
		lockCamera();

		const Uint8 *state = SDL_GetKeyboardState(NULL);
		if (state[SDL_SCANCODE_W])
		{
			if (camPitch != 90 && camPitch != -90)
				moveCamera(movevel, 0.0);
			moveCameraUp(movevel, 0.0);
		}
		else if (state[SDL_SCANCODE_S])
		{
			if (camPitch != 90 && camPitch != -90)
				moveCamera(movevel, 180.0);
			moveCameraUp(movevel, 180.0);
		}
		else if (state[SDL_SCANCODE_A])
		{
			moveCamera(movevel, 90.0);
		}
		else if (state[SDL_SCANCODE_D])
		{
			moveCamera(movevel, 270);
		}
		else if (state[SDL_SCANCODE_SPACE])
		{
			camY += movevel;
		}
		else if (state[SDL_SCANCODE_LSHIFT])
		{
			camY -= movevel;
		}

	}
	glRotatef(-camPitch, 1.0, 0.0, 0.0);       //rotate the camera (more precisly move everything in the opposit direction)
	glRotatef(-camYaw, 0.0, 1.0, 0.0);
}

void UpdateCamera()
{
	glTranslatef(-camX, -camY, -camZ);        //move the camera
}
