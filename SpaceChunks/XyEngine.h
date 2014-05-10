#ifndef XYENGINE_H
#define XYENGINE_H

#include <GL/glew.h>
#include <SDL/SDL.h>
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
#include "Time.h"
#include "SDL/SDL_image.h"

#ifdef HAVE_OPENGL
#include <SDL/SDL_opengl.h>
#endif

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
	void UpdateScene();

	double GetTime();

	SDL_Window* GetWindow();
	SDL_Event GetEvent();

	bool IsMouseIn();

	bool Running();

	void ClearScreen(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
	void ClearScreen(GLbitfield mask);
	void CreateList(GLuint id);
	void CallList(GLuint id);
	void EndList();

	void EnableImmediateMode(GLenum mode);
	void DisableImmediateMode();

	void ImmediateNormal(GLfloat x, GLfloat y, GLfloat z);
	void ImmediateVertex(GLfloat x, GLfloat y, GLfloat z);

	unsigned int LoadTexture(const char* filename)
	{
		unsigned int num;
		glGenTextures(1, &num);

		SDL_Surface* img = IMG_Load_RW(SDL_RWFromFile(filename, "rb"), 1);

		if (img == NULL)
		{
			printf("Unable to load bitmap: %s\n", SDL_GetError());
		}

		else{
			glBindTexture(GL_TEXTURE_2D, num);		
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->w, img->h, 0, GL_RGB, GL_UNSIGNED_BYTE, img->pixels);
			SDL_FreeSurface(img);
			return num;
		}
		return NULL;
	}

private:
	SDL_Window* m_Window;
	SDL_GLContext glcontext;
	SDL_Event event;
	bool m_running;
	bool mousein;
};
#endif

