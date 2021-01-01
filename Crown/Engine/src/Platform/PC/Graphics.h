#pragma once

#if defined(CROWN_PC)
#define CHECK_GL() CheckErrors(__FUNCTION__, __FILE__, __LINE__)
#else
#define CHECK_GL() 
#endif

namespace Crown {
	void CheckErrors(const char* function, const char* file, int line);
	bool CheckShader(unsigned int handle, const char* desc);
	bool CheckProgram(unsigned int handle, const char* desc);
}

