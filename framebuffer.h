#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H
#include <sstream>
#include <iostream>
#include <GL/glew.h>
#include <GL/gl.h>

bool checkFramebufferStatus();
void printFramebufferInfo();
std::string getRenderbufferParameters(GLuint id);
std::string getTextureParameters(GLuint id);
std::string convertInternalFormatToString(GLenum format);
#endif
