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

	int CreateWindow(int, int, char*, bool, int);
	void DestroyWindow();

	GLint GetShaderUniform(GLuint program, const GLchar *name);
	void SetShaderUniform(GLint location, GLfloat v_);
	void SetShaderUniform(GLint location, GLfloat v_, GLfloat v1);
	void SetShaderUniform(GLint location, GLfloat v_, GLfloat v1, GLfloat v2);
	void SetShaderUniform(GLint location, GLfloat v_, GLfloat v1, GLfloat v2, GLfloat v3);

	void RenderScene();

	double GetTime();

	GLFWwindow* GetWindow();

	bool Running();

private:
	GLFWwindow* m_Window;
};
#endif

