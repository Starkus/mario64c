#pragma once

#include <GL/glew.h>

class GLContext {
	static GLContext *s_instance;
	GLuint currentProgramID;

public:
	void setCurrentProgram(GLuint id);
	GLuint getCurrentProgram();
	GLuint getProgramUniform(const char*);
	static GLContext *instance();
};