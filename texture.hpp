#include <GL/gl.h>
#include <GL/glu.h>
#include <png.h>
#include <cstdio>
#include <string>
 
#define TEXTURE_LOAD_ERROR 0
 
  /** loadTexture
  *     loads a png file into an opengl texture object, using cstdio , libpng, and opengl.
  * 
  *     \param filename : the png file to be loaded
  *     \param width : width of png, to be updated as a side effect of this function
  *     \param height : height of png, to be updated as a side effect of this function
  * 
  *     \return GLuint : an opengl texture id.  Will be 0 if there is a major error,
  *                                     should be validated by the client of this function.
  * 
  */
 GLuint loadTexture(const std::string filename, int &width, int &height);
