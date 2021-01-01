#include "CrownPCH.h"

#include <GLFW/glfw3.h>

#include <iostream>

namespace Crown {

	static const char* getGlErrorString(GLenum err)
	{
		switch (err) {
		case GL_INVALID_OPERATION: return "GL_INVALID_OPERATION";
		case GL_INVALID_ENUM: return "GL_INVALID_ENUM";
		case GL_INVALID_VALUE: return "GL_INVALID_VALUE";
		case GL_OUT_OF_MEMORY: return "GL_OUT_OF_MEMORY";
		case GL_INVALID_FRAMEBUFFER_OPERATION: return "GL_INVALID_FRAMEBUFFER_OPERATION";
		default: return "Unknown OpenGL error";
		}
	}

	void CheckErrors(const char* function, const char* file, int line)
	{
		GLenum err(glGetError());
		while (err != GL_NO_ERROR) {
			const char* error = getGlErrorString(err);
			std::cerr << error
				<< " at " << function
				<< " in " << file << ":" << line
				<< std::endl;

			err = glGetError();
		}
	}

	bool CheckShader(GLuint handle, const char* desc)
	{
		GLint status = 0, logLength = 0;
		glGetShaderiv(handle, GL_COMPILE_STATUS, &status);
		glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &logLength);
		if (GLboolean(status) == GL_FALSE)
			fprintf(stderr, "ERROR: Shader failed to compile %s!\n", desc);
		if (logLength > 0) {
			std::vector<char> buf;
			buf.resize(logLength + 1);
			glGetShaderInfoLog(handle, logLength, nullptr, &buf[0]);
			fprintf(stderr, "%s\n", &buf[0]);
		}
		return GLboolean(status) == GL_TRUE;
	}

	bool CheckProgram(GLuint handle, const char* desc)
	{
		GLint status = 0, logLength = 0;
		glGetProgramiv(handle, GL_LINK_STATUS, &status);
		glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &logLength);
		if (GLboolean(status) == GL_FALSE)
			fprintf(stderr, "ERROR: Shader failed to link %s!\n", desc);
		if (logLength > 0) {
			std::vector<char> buf;
			buf.resize(logLength + 1);
			glGetProgramInfoLog(handle, logLength, nullptr, &buf[0]);
			fprintf(stderr, "%s\n", &buf[0]);
		}
		return GLboolean(status) == GL_TRUE;
	}

}
