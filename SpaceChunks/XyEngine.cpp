#include "XyEngine.h"


XyEngine::XyEngine()
{
}

GLFWwindow* XyEngine::GetWindow()
{
	return m_Window;
}

double XyEngine::GetTime()
{
	return glfwGetTime();
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

int XyEngine::CreateWindow(int width, int height, char* title, bool OpenGL_3_3_Enabled)
{
	printf("[XYENGINE] XyEngine is Loading... \n");

	if (!glfwInit())
	{
		printf("[CORE] GLFW Failed to Init! \n");
		return EXIT_FAILURE;
	}

	if (OpenGL_3_3_Enabled)
	{
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_FALSE);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		printf("[XYENGINE] XyEngine is Running in OpenGL 3.3+ Core Profie! \n");
	}
	else
	{
		printf("[XYENGINE] XyEngine is Running in Legacy Mode (Pre OpenGL 3.3)! \n");
	}


	m_Window = glfwCreateWindow(width, height, title, NULL, NULL);

	if (!m_Window)
	{
		glfwTerminate();
		printf("[CORE] GLFW Failed to Create the Window! \n");
		return EXIT_FAILURE;
	}


	glfwMakeContextCurrent(m_Window);

	if (glewInit() != GLEW_OK)
	{
		printf("[CORE] GLEW Failed To Init \n");
		return EXIT_FAILURE;
	}
}

bool XyEngine::Running()
{
	return !glfwWindowShouldClose(m_Window);
}

void XyEngine::RenderScene()
{
	glfwSwapBuffers(m_Window);
	glfwPollEvents();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void XyEngine::DestroyWindow()
{
	printf("[CORE] XyEngine is shutting down... \n");
	glfwDestroyWindow(m_Window);
	glfwTerminate();
}

XyEngine::~XyEngine()
{
	if (m_Window != NULL)
	{
		DestroyWindow();
	}
}
