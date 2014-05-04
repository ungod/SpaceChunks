#ifndef XYENGINE_H
#define XYENGINE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <cmath>
#include <cstring>
#include <algorithm> 
#include <time.h>  
#include <vector>
#include <fstream>
#include <string>

class XyEngine
{
public:
	XyEngine();
	~XyEngine();

	int CreateWindow(int width, int height, char* title, bool OpenGL_3_2_Enabled);
	void DestroyWindow();

	void RenderScene();

	GLFWwindow* GetWindow();

	bool Running();

private:
	GLFWwindow* m_Window;
};
#endif

