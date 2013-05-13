#ifndef GLSLPROGRMA_H
#define GLSLPROGRMA_H
#include <GL/glut.h>
#include "textfile.h"
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <string>
class GLSLProgram
{
  
 public:
  GLSLProgram(char* vs, GLuint& vsId, char* fs, GLuint& fsId, GLuint& programId, bool link);
  GLSLProgram(std::string vs, GLuint& vsId,std::string fs, GLuint& fsId, GLuint& programId, bool link);
  GLSLProgram(GLuint& programId);
  ~GLSLProgram();

  void setFragmentShader(char *fsFile, GLuint &fsId);
  void setVertexShader(char *vsFile, GLuint &vsId);

  void setFragmentShader( std::string fsFile, GLuint &fsId);
  void setVertexShader(std::string vsFile, GLuint &vsId);

  void linkProgram(GLuint& programId);

  int printOglError(char *file, int line);
  void printShaderInfoLog(GLuint obj);
  void printProgramInfoLog(GLuint obj);

  void useShaders(const GLuint& use);
  void attachVertexShader(const GLuint& shader, const GLuint& programId);
  void attachFragmentShader(const GLuint& shader, const GLuint& programId);
  void changeFragmentShader(const GLuint& shader, const GLuint& programId);
  void changeVertexShader(const GLuint& shader, const GLuint& programId);

  void setUniform1f(const GLuint& programa, GLuint loc, const char* nameLocation,GLfloat value); //regresa la location de value
  void setUniform2f(const GLuint& programa, GLuint loc, const char* nameLocation,GLfloat v0, GLfloat v1); //regresa la location de value
  void setUniform3f(const GLuint& programa, GLuint loc, const char* nameLocation,GLfloat v0, GLfloat v1, GLfloat v2); //regresa la location de value
  void setUniform4f(const GLuint& programa, GLuint loc, const char* nameLocation,GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3); //regresa la location de value
  void setUniform1fv(const GLuint& programa, GLuint loc, const char* nameLocation,GLfloat* value, int size);
  void setUniform2fv(const GLuint& programa, GLuint loc, const char* nameLocation,GLfloat* value, int size);
  void setUniform3fv(const GLuint& programa, GLuint loc, const char* nameLocation,GLfloat* value, int size);
  void setUniform4fv(const GLuint& programa, GLuint loc, const char* nameLocation,GLfloat* value, int size);
 
  void bindAttributeLocation(const GLuint& programa,unsigned int nameId, char* name);
  
  void setUniformMatrix4(const GLuint& programa,GLfloat* matrix, const char* nameLocation, GLuint loc);

private:
  GLuint lastVertexShader;
  GLuint lastFragmentShader;

};
#endif
