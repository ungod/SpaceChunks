#include "XyEngine.h"


XyEngine::XyEngine()
{
}

void XyEngine::ClearScreen(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
{
	glClearColor(red, green, blue, alpha);
}

void XyEngine::ClearScreen(GLbitfield mask)
{
	glClear(mask);
}

void XyEngine::CreateList(GLuint id)
{
	glNewList(id, GL_COMPILE);
}

void XyEngine::CallList(GLuint id)
{
	glCallList(id);
}

void XyEngine::EnableImmediateMode(GLenum mode)
{
	glBegin(mode);
}

void XyEngine::DisableImmediateMode()
{
	glEnd();
}

void XyEngine::EndList()
{
	glEndList();
}

void XyEngine::ImmediateNormal(GLfloat x, GLfloat y, GLfloat z)
{
	glNormal3f(x, y, z);
}

void XyEngine::ImmediateVertex(GLfloat x, GLfloat y, GLfloat z)
{
	glVertex3f(x, y, z);
}

SDL_Window* XyEngine::GetWindow()
{
	return m_Window;
}

double XyEngine::GetTime()
{
	return 0;
}

void XyEngine::SetShaderUniform(GLint location, GLfloat v_)
{
	return glUniform1f(location, v_);
}

void XyEngine::SetShaderUniform(GLint location, GLfloat v_, GLfloat v1)
{
	return glUniform2f(location, v_, v1);
}

void XyEngine::SetShaderUniform(GLint location, GLfloat v_, GLfloat v1, GLfloat v2)
{
	return glUniform3f(location, v_, v1, v2);
}

void XyEngine::SetShaderUniform(GLint location, GLfloat v_, GLfloat v1, GLfloat v2, GLfloat v3)
{
	return glUniform4f(location, v_, v1, v2, v3);
}

GLint XyEngine::GetShaderUniform(GLuint program, const GLchar *name)
{
	return glGetUniformLocation(program, name);

}

int XyEngine::CreateWindow(int width, int height, char* title, bool OpenGL_4_0_Enabled, int samples)
{
	printf("[XYENGINE] XyEngine is Loading... \n");

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		printf("[CORE] GLFW Failed to Init! \n");
		return EXIT_FAILURE;
	}

	if (OpenGL_4_0_Enabled)
	{
		//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
		//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		printf("[XYENGINE] XyEngine is Running in OpenGL 4.0 Core Profie! \n");
	}
	else
	{
		printf("[XYENGINE] XyEngine is Running in Legacy Mode (Pre OpenGL 4.0)! \n");
	}


	m_Window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL);

	if (m_Window == NULL)
	{
		SDL_Quit();
		printf("[CORE] GLFW Failed to Create the Window! \n");
		return EXIT_FAILURE;
	}


	glcontext = SDL_GL_CreateContext(m_Window);

	if (glewInit() != GLEW_OK)
	{
		printf("[CORE] GLEW Failed To Init \n");
		return EXIT_FAILURE;
	}

	m_running = true;
	return EXIT_SUCCESS;
}

bool XyEngine::Running()
{
	return m_running;
}

SDL_Event XyEngine::GetEvent()
{
	return event;
}

bool XyEngine::IsMouseIn()
{
	return mousein;
}

void XyEngine::DestroyWindow()
{
	printf("[CORE] XyEngine is shutting down... \n");
	SDL_GL_DeleteContext(glcontext);
	SDL_DestroyWindow(m_Window);
	SDL_Quit();
}

XyEngine::~XyEngine()
{
	if (m_Window != NULL)
	{
		DestroyWindow();
	}
}
