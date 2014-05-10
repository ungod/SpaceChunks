#ifndef SHADER_H
#define SHADER_H

#include "XyEngine.h"

class Shader
{
public:
	Shader(const std::string& fileName);
	GLuint GetShaderProgram();
	void Bind();
	void UnBind();
	virtual ~Shader();
protected:
private:
	static const unsigned int NUM_SHADERS = 2;

	std::string LoadShader(const std::string& fileName);
	void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);
	GLuint CreateShader(const std::string& text, unsigned int type);

	GLuint m_program;
	GLuint m_shaders[NUM_SHADERS];
};

#endif



