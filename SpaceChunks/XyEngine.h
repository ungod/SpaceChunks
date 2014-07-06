#ifndef XYENGINE_H
#define XYENGINE_H

#include <GL/glew.h>
#include <SDL/SDL.h>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
#define GLM_FORCE_RADIANS
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

#include "ShaderUtil.h"
#include "Matrices.h"

#ifndef M_PI
#define M_PI = 3.14159265358979
#endif

static double windowWidth = 1280;
static double windowHeight = 720;

static double MidX = windowWidth / 2;
static double MidY = windowHeight / 2;

#define CHUNK_X 16
#define CHUNK_Y 32
#define CHUNK_Z 16

class XyEngine
{
private:
	void(*m_InitFunc)();
	void(*m_RenderFunc)();
	void(*m_InputFunc)(SDL_Event event);
	bool m_FunctionSet = false;
	float m_FrameRate;

	SDL_Window* m_Window;
	SDL_GLContext glcontext;
	SDL_Event event;
	bool m_running;
	bool mousein;
	Matrices m_pipeline;
	GLuint m_posShader;
	unsigned int m_FBO;
	unsigned int m_RenderTexture;
	unsigned int m_RenderTextureDepth;

	float m_frameTime;
	float frames = 0;
	float fpsRate;
	int seed;
	static const unsigned int NUM_SHADERS = 2;
	GLuint m_shaders[NUM_SHADERS];
	GLuint text_program;
public:
	XyEngine();

	void SetFunctions(void(*initFunc)(), void(*renderFunc)(), void(*inputFunc)(SDL_Event event));
	int CreateWindow(int width, int height, char* title, float frameRate);
	int ReturnWithError(std::string err);
	void LoadShaderProgram(GLuint &shader, const std::string& fileName);

	Matrices GetPipeline()
	{
		return m_pipeline;
	}

	void AddUniformFloat(const GLchar *name​, float i)
	{
		glUniform1f(glGetUniformLocation(m_posShader, name​), i);
	}

	void AddUniformVector3(const GLchar *name​, glm::vec3 i)
	{
		glUniform3f(glGetUniformLocation(m_posShader, name​), i.x, i.y, i.z);
	}

	void AddUniformVector4(const GLchar *name​, glm::vec4 i)
	{
		glUniform4f(glGetUniformLocation(m_posShader, name​), i.x, i.y, i.z, i.w);
	}

	~XyEngine();


	void BindShader(GLuint &shader)
	{
		glUseProgram(shader);
	}
	void UnbindShader()
	{
		glUseProgram(0);
	}

	void DisposeShader(GLuint &shader)
	{
		for (unsigned int i = 0; i < NUM_SHADERS; i++)
		{
			glDetachShader(shader, m_shaders[i]);
			glDeleteShader(m_shaders[i]);
		}

		glDeleteProgram(shader);
	}
	

	int GetSeed()
	{
		return seed;
	}

	void GenSeed()
	{
	    srand(time(NULL));
		seed = rand() / 1000;
	}
	void Set3D()
	{
		m_pipeline.SetMatrixMode(PROJECTION_MATRIX);
		m_pipeline.LoadIndentity();
		m_pipeline.SetPerspective(70.0f, windowWidth / windowHeight, 0.01, 1000.0);
		m_pipeline.SetMatrixMode(MODEL_MATRIX);
		m_pipeline.LoadIndentity();

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHTING);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_CULL_FACE);
	}

	void Set2D()
	{
		m_pipeline.SetMatrixMode(PROJECTION_MATRIX);
		m_pipeline.LoadIndentity();
		m_pipeline.SetOrtho(0, windowWidth, windowHeight, 0, 0.01, 1000.0);
		m_pipeline.SetMatrixMode(MODEL_MATRIX);
		m_pipeline.LoadIndentity();

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_CULL_FACE);
	}

	void UpdateMatrix()
	{
		m_pipeline.UpdateMatrices(m_posShader);
	}

	void ClearMatrix()
	{
		m_pipeline.LoadIndentity();
	}

	void SetMatrix(int i)
	{
		m_pipeline.SetMatrixMode(i);
	}

	void TranslateWorldMatrix(float x, float y, float z)
	{
		m_pipeline.Translate(x, y, z);
	}

	void TranslateWorldMatrix(glm::vec3 pos)
	{
		m_pipeline.Translate(pos.x, pos.y, pos.z);
	}

	void PushMatrix()
	{
		m_pipeline.PushMatrix();
	}

	void PopMatrix()
	{
		m_pipeline.PopMatrix();
	}

	void RotateWorldMatrix_X(float angle)
	{
		m_pipeline.RotateX(angle);
	}

	void RotateWorldMatrix_Y(float angle)
	{
		m_pipeline.RotateY(angle);
	}

	void RotateWorldMatrix_Z(float angle)
	{
		m_pipeline.RotateZ(angle);
	}

	std::string ConvertIntToString(int num)
	{
		std::string str;
		std::ostringstream con;

		con << num;

		str = con.str();

		return str;
	}

	GLuint GetPosShader()
	{
		return m_posShader;
	}

	float GetFPS()
	{
		return fpsRate;
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

	unsigned int CreateTexture(int w, int h, bool isDepth = false)
	{
		unsigned int textureID;
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, (!isDepth ? GL_RGBA8 : GL_DEPTH_COMPONENT), w, h, 0, isDepth ? GL_DEPTH_COMPONENT : GL_RGBA, GL_FLOAT, NULL);;

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		int i = 0;
		i = glGetError();
		if (i != 0)
		{
			std::cout << "Error happened while creating the texture: " << i << std::endl;
		}
		glBindTexture(GL_TEXTURE_2D, 0);
		return textureID;
	}

	void RenderText(const TTF_Font *Font, const double& X, const double& Y, const std::string& Text)
	{
		glEnable(GL_TEXTURE_2D);
		/*Create some variables.*/

		m_pipeline.LoadIndentity();

		int m_attributeCoord = glGetAttribLocation(text_program, "coord");
		glEnableVertexAttribArray(m_attributeCoord);

		BindShader(text_program);

		SDL_Color Color = { 255, 255, 255 };
		SDL_Surface *Message = TTF_RenderText_Blended(const_cast<TTF_Font*>(Font), Text.c_str(), Color);
		unsigned int Texture = 0;

		int loc = glGetUniformLocation(text_program, "tex");


		/*Generate an OpenGL 2D texture from the SDL_Surface*.*/
		glActiveTexture(GL_TEXTURE1);
		glGenTextures(1, &Texture);
		glBindTexture(GL_TEXTURE_2D, Texture);
		glUniform1i(loc, 0);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		

		GLuint vbo;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glVertexAttribPointer(m_attributeCoord, 4, GL_FLOAT, GL_FALSE, 0, 0);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, Message->w, Message->h, 0, GL_ALPHA,
			GL_UNSIGNED_BYTE, Message->pixels);

		/*Draw this texture on a quad with the given xyz coordinates.*/

		GLfloat box[4][4] = 
		{
				{ X               , Y               , 0, 0 },
				{ X + Message->w  , Y               , 1, 0 },
				{ X + Message->w  , Y + Message->h  , 1, 1 },
				{ X               , Y + Message->h  , 0, 1 },
		};

		glBufferData(GL_ARRAY_BUFFER, sizeof box, box, GL_DYNAMIC_DRAW);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		UnbindShader();

		/*Clean up.*/
		glDeleteTextures(1, &Texture);
		SDL_FreeSurface(Message);
		glDisable(GL_TEXTURE_2D);
	}

	void BindTexture(GLuint &tex, int i)
	{
		glEnable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE + i);
		glBindTexture(GL_TEXTURE_2D, tex);
	}

	void UnbindTexture()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
	}

	void DisposeTexture(GLuint &tex)
	{
		glDeleteTextures(1, &tex);
	}

	void LoadTexture(GLuint &tex, const char* filename)
	{
		glGenTextures(1, &tex);

		SDL_Surface* img = IMG_Load_RW(SDL_RWFromFile(filename, "rb"), 1);

		if (img == NULL)
		{
			printf("Unable to load Image: %s\n", SDL_GetError());
			tex = NULL;
		}

		else{
			glBindTexture(GL_TEXTURE_2D, tex);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img->w, img->h, 0, GL_RGB, GL_UNSIGNED_BYTE, img->pixels);
			SDL_FreeSurface(img);
		}
	}

	bool IsFaceYInView(int x, int y, int z, bool reversed, uint32_t m_blocks[CHUNK_X][CHUNK_Y][CHUNK_Z])
	{
		bool faceHidden;

		if (reversed == false)
		{
			if (y > 0) {
				if (m_blocks[x][y - 1][z] == 0) faceHidden = false;
				else faceHidden = true;
			}
			else {
				faceHidden = false;
			}
			return faceHidden;
		}
		else {
			if (y < CHUNK_Y - 1) {
				if (m_blocks[x][y + 1][z] == 0) faceHidden = false;
				else faceHidden = true;
			}
			else {
				faceHidden = false;
			}
			return faceHidden;
		}
	}

	bool IsFaceXInView(int x, int y, int z, bool reversed, uint32_t m_blocks[CHUNK_X][CHUNK_Y][CHUNK_Z])
	{
		bool faceHidden;

		if (reversed == false)
		{
			if (x > 0) {
				if (m_blocks[x - 1][y][z] == 0) faceHidden = false;
				else faceHidden = true;
			}
			else {
				faceHidden = false;
			}
			return faceHidden;
		}
		else {
			if (x < CHUNK_X - 1) {
				if (m_blocks[x + 1][y][z] == 0) faceHidden = false;
				else faceHidden = true;
			}
			else {
				faceHidden = false;
			}
			return faceHidden;
		}
	}

	bool IsFaceZInView(int x, int y, int z, bool reversed, uint32_t m_blocks[CHUNK_X][CHUNK_Y][CHUNK_Z])
	{
		bool faceHidden;

		if (reversed == false)
		{
			if (z > 0) {
				if (m_blocks[x][y][z - 1] == 0) faceHidden = false;
				else faceHidden = true;
			}
			else {
				faceHidden = false;
			}
			return faceHidden;
		}
		else {
			if (z < CHUNK_Z - 1) {
				if (m_blocks[x][y][z + 1] == 0) faceHidden = false;
				else faceHidden = true;
			}
			else {
				faceHidden = false;
			}
			return faceHidden;
		}
	}

};
#endif