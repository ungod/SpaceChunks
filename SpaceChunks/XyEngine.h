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
#include <SDL/SDL_thread.h>
#include <sstream>
#include <functional>
#include <Noise/simplexnoise.h>
#include <thread>

#ifdef HAVE_OPENGL
#include <SDL/SDL_opengl.h>
#endif

static double windowWidth = 1280;
static double windowHeight = 720;

static double MidX = windowWidth / 2;
static double MidY = windowHeight / 2;

extern bool ShowGrid;

class XyEngine
{
private:
	SDL_Window* m_Window;
	SDL_GLContext glcontext;
	SDL_Event event;
	bool m_running;
	bool mousein;

	float frameRate=60;

	float m_frameTime = 1.0 / frameRate;

	float frames = 0;

	float fpsRate;
	int seed;

	

public:

	

	int GetSeed()
	{
		return seed;
	}
	void Set3D()
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(60.0, windowWidth / windowHeight, 0.01, 1000.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glEnable(GL_DEPTH_TEST);
	}

	void Set2D()
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(0, windowWidth, windowHeight, 0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_CULL_FACE);
	}

	void setFPS(float framerate)
	{
		frameRate = framerate;
	}

	std::string ConvertIntToString(int num)
	{
		std::string str;
		std::ostringstream con;

		con << num;

		str = con.str();

		return str;
	}

	XyEngine()
	{
		srand(time(NULL));
		seed = rand() % 100;
	}

	~XyEngine()
	{
		if (m_Window != NULL)
		{
			DestroyWindow();
		}
	}

	/* Width, Height, Title, Init Function, Render Function */
	int CreateWindow(int width, int height, char* title, float frameRateIn, void(*initFunc)(), void(*renderFunc)(), void(*inputFunc)(SDL_Event event))
	{
		printf("[XYENGINE] XyEngine is Loading... \n");

		frameRate = NULL;
		setFPS(frameRateIn);

		if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
		{
			printf("[CORE] SDL Failed to Init! \n");
			return EXIT_FAILURE;
		}

		m_Window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL);

		if (m_Window == NULL)
		{
			SDL_Quit();
			printf("[CORE] SDL Failed to Create the Window! \n");
			return EXIT_FAILURE;
		}


		glcontext = SDL_GL_CreateContext(m_Window);

		if (glewInit() != GLEW_OK)
		{
			printf("[CORE] GLEW Failed To Init \n");
			return EXIT_FAILURE;
		}

		if (TTF_Init() != 0)
		{
			printf("[CORE] SDL TTF Failed to Init! \n");
			return EXIT_FAILURE;
		}

		if (!IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG))
		{
			printf("[CORE] SDL Image Failed to Init! \n");
			return EXIT_FAILURE;
		}

		m_running = true;

		initFunc();

		double lastTime = Time::GetTime();
		double unprocessedTime = 0;
		double frameCounter = 0;
		frames = 0;

		SDL_Event event;

		while (m_running)
		{
			bool render = false;

			double startTime = Time::GetTime();
			double passedTime = startTime - lastTime;
			lastTime = startTime;

			unprocessedTime += passedTime;
			frameCounter += passedTime;

			if (frameCounter >= 1.0)
			{
				fpsRate = frames;
				frames = 0;
				frameCounter = 0;
			}

			while (unprocessedTime > m_frameTime)
			{
				render = true;

				while (SDL_PollEvent(&event))
				{
					inputFunc(event);

					switch (event.type)
					{

					case SDL_QUIT:
						m_running = false;
						break;

					case SDL_MOUSEBUTTONDOWN:
						mousein = true;
						SDL_ShowCursor(SDL_DISABLE);
						break;

					case SDL_KEYDOWN:
						switch (event.key.keysym.sym)
						{
						case SDLK_ESCAPE:
							mousein = false;
							SDL_ShowCursor(SDL_ENABLE);
							break;
						}
					}
				}

				unprocessedTime -= m_frameTime;
			}

			if (render)
			{
				if (mousein)
					SDL_WarpMouseInWindow(m_Window, (int)MidX, (int)MidY);

				//Render();
				renderFunc();
				SDL_GL_SwapWindow(m_Window);
				frames++;
			}
			else
			{
				SDL_Delay(1);
			}
		}


		return EXIT_SUCCESS;
	}

	float GetFPS()
	{
		return fpsRate;
	}

	void DestroyWindow()
	{
		printf("[CORE] XyEngine is shutting down... \n");
		TTF_Quit();
		IMG_Quit();
		SDL_GL_DeleteContext(glcontext);
		SDL_DestroyWindow(m_Window);
		SDL_Quit();
	}

	GLint GetShaderUniform(GLuint program, const GLchar *name)
	{
		return glGetUniformLocation(program, name);
	}

	void SetShaderUniform(GLint location, GLfloat v_)
	{
		return glUniform1f(location, v_);
	}

	void SetShaderUniform(GLint location, GLfloat v_, GLfloat v1)
	{
		return glUniform2f(location, v_, v1);
	}

	void SetShaderUniform(GLint location, GLfloat v_, GLfloat v1, GLfloat v2)
	{
		return glUniform3f(location, v_, v1, v2);
	}

	void SetShaderUniform(GLint location, GLfloat v_, GLfloat v1, GLfloat v2, GLfloat v3)
	{
		return glUniform4f(location, v_, v1, v2, v3);
	}

	SDL_Window* GetWindow()
	{
		return m_Window;
	}

	SDL_Event GetEvent()
	{
		return event;
	}

	bool IsMouseIn()
	{
		return mousein;
	}

	bool Running()
	{
		return m_running;
	}

	void ClearScreen(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
	{
		glClearColor(red, green, blue, alpha);
	}

	void ClearScreen(GLbitfield mask)
	{
		glClear(mask);
	}

	void CreateList(GLuint id)
	{
		glNewList(id, GL_COMPILE);
	}

	void CallList(GLuint id)
	{
		glCallList(id);
	}

	void EndList()
	{
		glEndList();
	}

	void EnableImmediateMode(GLenum mode)
	{
		glBegin(mode);
	}

	void DisableImmediateMode()
	{
		glEnd();
	}

	void ImmediateNormal(GLfloat x, GLfloat y, GLfloat z)
	{
		glNormal3f(x, y, z);
	}

	void ImmediateVertex(GLfloat x, GLfloat y, GLfloat z)
	{
		glVertex3f(x, y, z);
	}

	
	void RenderText(const TTF_Font *Font, const double& X, const double& Y, const std::string& Text)
	{
		glEnable(GL_TEXTURE_2D);
		/*Create some variables.*/
		SDL_Color Color = { 255, 255, 255 };
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
		glTexCoord2d(0, 0); glVertex3d(X, Y, 0);
		glTexCoord2d(1, 0); glVertex3d(X + Message->w, Y, 0);
		glTexCoord2d(1, 1); glVertex3d(X + Message->w, Y + Message->h, 0);
		glTexCoord2d(0, 1); glVertex3d(X, Y + Message->h, 0);
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
};
#endif