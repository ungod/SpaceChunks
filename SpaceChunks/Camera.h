#include <math.h>

#ifndef CAMERA_H
#define CAMERA_H

void lockCamera();
void moveCamera(float, float);
void moveCameraUp(float, float);
void Control(float, float, bool);
void UpdateCamera();

float getCamX();
float getCamY();
float getCamZ();

void setCamX(float);
void setCamY(float);
void setCamZ(float);
#endif



