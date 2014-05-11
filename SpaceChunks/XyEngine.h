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
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

#ifdef HAVE_OPENGL
#include <SDL/SDL_opengl.h>
#endif

static double windowWidth = 1280;
static double windowHeight = 720;

static double MidX = windowWidth / 2;
static double MidY = windowHeight / 2;

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

	void SDL_GL_RenderText(char *text,
		TTF_Font *font,
		SDL_Color color,
		SDL_Rect *location)
	{
		SDL_Surface *initial;
		SDL_Surface *intermediary;
		SDL_Rect rect;
		int w, h;
		GLuint texture;

		/* Use SDL_TTF to render our text */
		initial = TTF_RenderText_Blended(font, text, color);

		/* Convert the rendered text to a known format */
		w = initial->w;
		h = initial->h;

		intermediary = SDL_CreateRGBSurface(0, w, h, 32,
			0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);

		SDL_BlitSurface(initial, 0, intermediary, 0);

		/* Tell GL about our new texture */
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, w, h, 0, GL_BGRA,
			GL_UNSIGNED_BYTE, intermediary->pixels);

		/* GL_NEAREST looks horrible, if scaled... */
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		/* prepare to render our texture */
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture);
		glColor3f(1.0f, 1.0f, 1.0f);

		/* Draw a quad at location */
		glBegin(GL_QUADS);
		/* Recall that the origin is in the lower-left corner
		That is why the TexCoords specify different corners
		than the Vertex coors seem to. */
		glTexCoord2f(0.0f, 0.0f);  glVertex2f(location->x, location->y);
		glTexCoord2f(1.0f, 0.0f);  glVertex2f(location->x + w, location->y);
		glTexCoord2f(1.0f, 1.0f);  glVertex2f(location->x + w, location->y + h);
		glTexCoord2f(0.0f, 1.0f);  glVertex2f(location->x, location->y + h);
		glEnd();

		glFinish();

		location->w = initial->w;
		location->h = initial->h;

		SDL_FreeSurface(initial);
		SDL_FreeSurface(intermediary);
		glDeleteTextures(1, &texture);
	}

	void RenderText(const TTF_Font *Font, const GLubyte& R, const GLubyte& G, const GLubyte& B,
		const double& X, const double& Y, const double& Z, const std::string& Text)
	{
		glEnable(GL_TEXTURE_2D);
		/*Create some variables.*/
		SDL_Color Color = { R, G, B };
		SDL_Surface *Message = TTF_RenderText_Blended(const_cast<TTF_Font*>(Font), Text.c_str(), Color);
		unsigned Texture = 0;

		/*Generate an OpenGL 2D texture from the SDL_Surface*.*/
		glGenTextures(1, &Texture);
		glBindTexture(GL_TEXTURE_2D, Texture);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Message->w, Message->h, 0, GL_BGRA,
			GL_UNSIGNED_BYTE, Message->pixels);

		/*Draw this texture on a quad with the given xyz coordinates.*/
		glBegin(GL_QUADS);
		glTexCoord2d(0, 0); glVertex3d(X, Y, Z);
		glTexCoord2d(1, 0); glVertex3d(X + Message->w, Y, Z);
		glTexCoord2d(1, 1); glVertex3d(X + Message->w, Y + Message->h, Z);
		glTexCoord2d(0, 1); glVertex3d(X, Y + Message->h, Z);
		glEnd();

		/*Clean up.*/
		glDeleteTextures(1, &Texture);
		SDL_FreeSurface(Message);
		glDisable(GL_TEXTURE_2D);
	}

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
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img->w, img->h, 0, GL_RGB, GL_UNSIGNED_BYTE, img->pixels);
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

