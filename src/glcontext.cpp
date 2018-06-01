#include "glcontext.hpp"

GLContext* GLContext::instance() {
	if (!s_instance)
		s_instance = new GLContext;
	return s_instance;
}

GLContext *GLContext::s_instance = 0;


void GLContext::setCurrentProgram(GLuint id) {
	currentProgramID = id;
}

GLuint GLContext::getCurrentProgram() {
	return currentProgramID;
}

GLuint GLContext::getProgramUniform(const char* name) {
	return glGetUniformLocation(currentProgramID, name);
}