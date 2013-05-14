
#include <vector>
#include <iostream>

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/glu.h>
//#include "glext.h"
#include "glInfo.h"
#include "glslProgram.h"
#include "texture.hpp"
#include "framebuffer.h"

int screen_width, screen_height;

const int   TEXTURE_WIDTH   = 640;  // NOTE: texture size cannot be larger than WINDOW SIZE
const int   TEXTURE_HEIGHT  = 480;

const int   WINDOW_WIDTH   = 800;  // 
const int   WINDOW_HEIGHT  = 600;
GLfloat blanco[4] = {1.0,1.0,1.0,1.0};
bool useShader=false , useDisplacement = false;
bool fboSupported = false;
bool fboUsed = false;
bool shaderON = false,  phongON = false;
const int numTextures = 5;
GLuint textures[numTextures];
GLuint texture4Render, tID, framebuffer, fboID,renderBuffer, rboID;
float angle = 0.0;

GLuint projMatrixLoc, viewMatrixLoc;
const char* varyings[4] = {"vPosition", "vNormal", "vTexCoord","vColor"};
float projMatrix[16];
float viewMatrix[16];
float eyeX = 0.0, eyeY = 5.0, eyeZ = 10.0;
float dirX = 0.0, dirY = 5.0, dirZ = 0.0;

GLfloat v1[] = {10,10,10}; 
GLfloat v2[] = {10,10,-10}; 
GLfloat v3[] = {10,0,-10}; 
GLfloat v4[] = {10,0,10}; 
GLfloat v5[] = {-10,10,-10}; 
GLfloat v6[] = {-10,10,10}; 
GLfloat v7[] = {-10,0,10}; 
GLfloat v8[] = {-10,0,-10}; 

using namespace std;

void initRenderbufferTexture()
{
	
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &texture4Render);
    glBindTexture(GL_TEXTURE_2D, texture4Render);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); // automatic mipmap generation included in OpenGL v1.4
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    
	/*FRAMEBUFFER */
	glGenFramebuffers(1,&fboID);	
	glBindFramebuffer(GL_FRAMEBUFFER,fboID);
	glGenRenderbuffers(1,&rboID);
	glBindRenderbuffer(GL_RENDERBUFFER,rboID);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, TEXTURE_WIDTH, TEXTURE_HEIGHT);
	glBindRenderbuffer(GL_RENDERBUFFER,0);
	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,texture4Render,0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER,rboID);
	 
	
	
	bool status = checkFramebufferStatus();
	if(!status){
      fboUsed = false;
      
	exit(0);
	}
		
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_TEXTURE_2D);
}

void initTexture()
{
	int w, h, w1, h1, w2, h2, w3, h3;
	textures[0] = loadTexture("textures/wood_floor.png", w,h);
	textures[1] = loadTexture("textures/brick_wall.png", w1,h1);
	textures[2] = loadTexture("textures/ceil.png", w2,h2);
	textures[3] = loadTexture("textures/wood.png", w3,h3);
	initRenderbufferTexture();
}

void table()
{
	glBindTexture(GL_TEXTURE_2D, textures[3]);
	 glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	  /*
		6------------4
	   /|           /|
	  / |          / |
	 8------------2  |
	 |  5---------|--3
	 | /          | /
	 |/           |/
	 7----------- 1
	*/	
	//CUBIERTA
	glBegin(GL_QUAD_STRIP);
	//Quads 1, 2, 3 y 4
	glTexCoord2f(0.0, 1.0); glVertex3f( 2.0, 0.0, 4.0);  //V2
	glTexCoord2f(0.0, 0.0); glVertex3f( 2.0,-0.5, 4.0);  //V1
	glTexCoord2f(1.0, 1.0); glVertex3f( 2.0, 0.0,-4.0);  //V4
	glTexCoord2f(1.0, 0.0); glVertex3f( 2.0,-0.5,-4.0);  //V3
	glTexCoord2f(2.0, 1.0); glVertex3f(-2.0, 0.0,-4.0);  //V6
	glTexCoord2f(2.0, 0.0); glVertex3f(-2.0,-0.5,-4.0);  //V5
	glTexCoord2f(3.0, 1.0); glVertex3f(-2.0, 0.0, 4.0);  //V8
	glTexCoord2f(3.0, 0.0); glVertex3f(-2.0,-0.5, 4.0);  //V7
	glTexCoord2f(4.0, 1.0); glVertex3f( 2.0, 0.0, 4.0);  //V2
	glTexCoord2f(4.0, 0.0); glVertex3f( 2.0,-0.5, 4.0);  //V1
	glEnd();
	glBegin(GL_QUADS);
	//Quad 5 (Cubierta)
	glTexCoord2f(0.0, 1.0); glVertex3f(-2.0, 0.0,-4.0); //V6
	glTexCoord2f(0.0, 0.0); glVertex3f(-2.0, 0.0, 4.0); //V8
	glTexCoord2f(1.0, 0.0); glVertex3f( 2.0, 0.0, 4.0); //V2
	glTexCoord2f(1.0, 1.0); glVertex3f( 2.0, 0.0,-4.0); //V4
	//Quad 6 (Cara inferior)
	glTexCoord2f(0.0, 1.0); glVertex3f(-2.0,-0.5, 4.0); //V7
	glTexCoord2f(0.0, 0.0); glVertex3f(-2.0,-0.5,-4.0); //V5
	glTexCoord2f(1.0, 0.0); glVertex3f( 2.0,-0.5,-4.0); //V3
	glTexCoord2f(1.0, 1.0); glVertex3f( 2.0,-0.5, 4.0); //V1
	glEnd();
	
	//PATA DELANTERA IZQUIERDA
	glBegin(GL_QUAD_STRIP);
	//Quads 1, 2, 3 y 4
	glTexCoord2f(0.0, 1.0); glVertex3f(-1.0, -0.5, 3.5);  //V2
	glTexCoord2f(0.0, 0.0); glVertex3f(-1.0, -3.5, 3.5);  //V1
	glTexCoord2f(1.0, 1.0); glVertex3f(-1.0, -0.5, 3.0);  //V4
	glTexCoord2f(1.0, 0.0); glVertex3f(-1.0, -3.5, 3.0);  //V3
	glTexCoord2f(2.0, 1.0); glVertex3f(-1.5, -0.5, 3.0);  //V6
	glTexCoord2f(2.0, 0.0); glVertex3f(-1.5, -3.5, 3.0);  //V5
	glTexCoord2f(3.0, 1.0); glVertex3f(-1.5, -0.5, 3.5);  //V8
	glTexCoord2f(3.0, 0.0); glVertex3f(-1.5, -3.5, 3.5);  //V7
	glTexCoord2f(4.0, 1.0); glVertex3f(-1.0, -0.5, 3.5);  //V2
	glTexCoord2f(4.0, 0.0); glVertex3f(-1.0, -3.5, 3.5);  //V1
	glEnd();
	glBegin(GL_QUADS);
	//Quad 5 (Cubierta)
	glTexCoord2f(0.0, 1.0); glVertex3f(-1.5, -0.5, 3.0); //V6
	glTexCoord2f(0.0, 0.0); glVertex3f(-1.5, -0.5, 3.5); //V8
	glTexCoord2f(1.0, 0.0); glVertex3f(-1.0, -0.5, 3.5); //V2
	glTexCoord2f(1.0, 1.0); glVertex3f(-1.0, -0.5, 3.0); //V4
	//Quad 6 (Cara inferior)
	glTexCoord2f(0.0, 1.0); glVertex3f(-1.5, -3.5, 3.5); //V7
	glTexCoord2f(0.0, 0.0); glVertex3f(-1.5, -3.5, 3.0); //V5
	glTexCoord2f(1.0, 0.0); glVertex3f(-1.0, -3.5, 3.0); //V3
	glTexCoord2f(1.0, 1.0); glVertex3f(-1.0, -3.5, 3.5); //V1
	glEnd();	
	
	//PATA DELANTERA DERECHA
	glBegin(GL_QUAD_STRIP);
	//Quads 1, 2, 3 y 4
	glTexCoord2f(0.0, 1.0); glVertex3f(1.5, -0.5, 3.5);  //V2
	glTexCoord2f(0.0, 0.0); glVertex3f(1.5, -3.5, 3.5);  //V1
	glTexCoord2f(1.0, 1.0); glVertex3f(1.5, -0.5, 3.0);  //V4
	glTexCoord2f(1.0, 0.0); glVertex3f(1.5, -3.5, 3.0);  //V3
	glTexCoord2f(2.0, 1.0); glVertex3f(1.0, -0.5, 3.0);  //V6
	glTexCoord2f(2.0, 0.0); glVertex3f(1.0, -3.5, 3.0);  //V5
	glTexCoord2f(3.0, 1.0); glVertex3f(1.0, -0.5, 3.5);  //V8
	glTexCoord2f(3.0, 0.0); glVertex3f(1.0, -3.5, 3.5);  //V7
	glTexCoord2f(4.0, 1.0); glVertex3f(1.5, -0.5, 3.5);  //V2
	glTexCoord2f(4.0, 0.0); glVertex3f(1.5, -3.5, 3.5);  //V1
	glEnd();
	glBegin(GL_QUADS);
	//Quad 5 (Cubierta)
	glTexCoord2f(0.0, 1.0); glVertex3f(1.0, -0.5, 3.0); //V6
	glTexCoord2f(0.0, 0.0); glVertex3f(1.0, -0.5, 3.5); //V8
	glTexCoord2f(1.0, 0.0); glVertex3f(1.5, -0.5, 3.5); //V2
	glTexCoord2f(1.0, 1.0); glVertex3f(1.5, -0.5, 3.0); //V4
	//Quad 6 (Cara inferior)
	glTexCoord2f(0.0, 1.0); glVertex3f(1.0, -3.5, 3.5); //V7
	glTexCoord2f(0.0, 0.0); glVertex3f(1.0, -3.5, 3.0); //V5
	glTexCoord2f(1.0, 0.0); glVertex3f(1.5, -3.5, 3.0); //V3
	glTexCoord2f(1.0, 1.0); glVertex3f(1.5, -3.5, 3.5); //V1
	glEnd();	
	
	//PATA TRASERA IZQUIERDA
	glBegin(GL_QUAD_STRIP);
	//Quads 1, 2, 3 y 4
	glTexCoord2f(0.0, 1.0); glVertex3f(-1.0, -0.5, -3.0);  //V2
	glTexCoord2f(0.0, 0.0); glVertex3f(-1.0, -3.5, -3.0);  //V1
	glTexCoord2f(1.0, 1.0); glVertex3f(-1.0, -0.5, -3.5);  //V4
	glTexCoord2f(1.0, 0.0); glVertex3f(-1.0, -3.5, -3.5);  //V3
	glTexCoord2f(2.0, 1.0); glVertex3f(-1.5, -0.5, -3.5);  //V6
	glTexCoord2f(2.0, 0.0); glVertex3f(-1.5, -3.5, -3.5);  //V5
	glTexCoord2f(3.0, 1.0); glVertex3f(-1.5, -0.5, -3.0);  //V8
	glTexCoord2f(3.0, 0.0); glVertex3f(-1.5, -3.5, -3.0);  //V7
	glTexCoord2f(4.0, 1.0); glVertex3f(-1.0, -0.5, -3.0);  //V2
	glTexCoord2f(4.0, 0.0); glVertex3f(-1.0, -3.5, -3.0);  //V1
	glEnd();
	glBegin(GL_QUADS);
	//Quad 5 (Cubierta)
	glTexCoord2f(0.0, 1.0); glVertex3f(-1.5, -0.5, -3.5); //V6
	glTexCoord2f(0.0, 0.0); glVertex3f(-1.5, -0.5, -3.0); //V8
	glTexCoord2f(1.0, 0.0); glVertex3f(-1.0, -0.5, -3.0); //V2
	glTexCoord2f(1.0, 1.0); glVertex3f(-1.0, -0.5, -3.5); //V4
	//Quad 6 (Cara inferior)
	glTexCoord2f(0.0, 1.0); glVertex3f(-1.5, -3.5, -3.0); //V7
	glTexCoord2f(0.0, 0.0); glVertex3f(-1.5, -3.5, -3.5); //V5
	glTexCoord2f(1.0, 0.0); glVertex3f(-1.0, -3.5, -3.5); //V3
	glTexCoord2f(1.0, 1.0); glVertex3f(-1.0, -3.5, -3.0); //V1
	glEnd();		
	
	//PATA TRASERA DERECHA
	glBegin(GL_QUAD_STRIP);
	//Quads 1, 2, 3 y 4
	glTexCoord2f(0.0, 1.0); glVertex3f(1.5, -0.5, -3.0);  //V2
	glTexCoord2f(0.0, 0.0); glVertex3f(1.5, -3.5, -3.0);  //V1
	glTexCoord2f(1.0, 1.0); glVertex3f(1.5, -0.5, -3.5);  //V4
	glTexCoord2f(1.0, 0.0); glVertex3f(1.5, -3.5, -3.5);  //V3
	glTexCoord2f(2.0, 1.0); glVertex3f(1.0, -0.5, -3.5);  //V6
	glTexCoord2f(2.0, 0.0); glVertex3f(1.0, -3.5, -3.5);  //V5
	glTexCoord2f(3.0, 1.0); glVertex3f(1.0, -0.5, -3.0);  //V8
	glTexCoord2f(3.0, 0.0); glVertex3f(1.0, -3.5, -3.0);  //V7
	glTexCoord2f(4.0, 1.0); glVertex3f(1.5, -0.5, -3.0);  //V2
	glTexCoord2f(4.0, 0.0); glVertex3f(1.5, -3.5, -3.0);  //V1
	glEnd();
	glBegin(GL_QUADS);
	//Quad 5 (Cubierta)
	glTexCoord2f(0.0, 1.0); glVertex3f(1.0, -0.5, -3.5); //V6
	glTexCoord2f(0.0, 0.0); glVertex3f(1.0, -0.5, -3.0); //V8
	glTexCoord2f(1.0, 0.0); glVertex3f(1.5, -0.5, -3.0); //V2
	glTexCoord2f(1.0, 1.0); glVertex3f(1.5, -0.5, -3.5); //V4
	//Quad 6 (Cara inferior)
	glTexCoord2f(0.0, 1.0); glVertex3f(1.0, -3.5, -3.0); //V7
	glTexCoord2f(0.0, 0.0); glVertex3f(1.0, -3.5, -3.5); //V5
	glTexCoord2f(1.0, 0.0); glVertex3f(1.5, -3.5, -3.5); //V3
	glTexCoord2f(1.0, 1.0); glVertex3f(1.5, -3.5, -3.0); //V1
	glEnd();
	
	//LARGUERO DELANTERO
	glBegin(GL_QUAD_STRIP);
	//Quads 1, 2, 3 y 4
	glTexCoord2f(0.0, 1.0); glVertex3f(1.0, -0.5, 3.5);  //V2
	glTexCoord2f(0.0, 0.0); glVertex3f(1.0, -1.0, 3.5);  //V1
	glTexCoord2f(1.0, 1.0); glVertex3f(1.0, -0.5, 3.0);  //V4
	glTexCoord2f(1.0, 0.0); glVertex3f(1.0, -1.0, 3.0);  //V3
	glTexCoord2f(2.0, 1.0); glVertex3f(-1.0, -0.5, 3.0);  //V6
	glTexCoord2f(2.0, 0.0); glVertex3f(-1.0, -1.0, 3.0);  //V5
	glTexCoord2f(3.0, 1.0); glVertex3f(-1.0, -0.5, 3.5);  //V8
	glTexCoord2f(3.0, 0.0); glVertex3f(-1.0, -1.0, 3.5);  //V7
	glTexCoord2f(4.0, 1.0); glVertex3f(1.0, -0.5, 3.5);  //V2
	glTexCoord2f(4.0, 0.0); glVertex3f(1.0, -1.0, 3.5);  //V1
	glEnd();
	glBegin(GL_QUADS);
	//Quad 5 (Cubierta)
	glTexCoord2f(0.0, 1.0); glVertex3f(-1.0, -0.5, 3.0); //V6
	glTexCoord2f(0.0, 0.0); glVertex3f(-1.0, -0.5, 3.5); //V8
	glTexCoord2f(1.0, 0.0); glVertex3f(1.0, -0.5, 3.5); //V2
	glTexCoord2f(1.0, 1.0); glVertex3f(1.0, -0.5, 3.0); //V4
	//Quad 6 (Cara inferior)
	glTexCoord2f(0.0, 1.0); glVertex3f(-1.0, -1.0, 3.5); //V7
	glTexCoord2f(0.0, 0.0); glVertex3f(-1.0, -1.0, 3.0); //V5
	glTexCoord2f(1.0, 0.0); glVertex3f(1.0, -1.0, 3.0); //V3
	glTexCoord2f(1.0, 1.0); glVertex3f(1.0, -1.0, 3.5); //V1
	glEnd();
	
	//LARGUERO TRASERO
	glBegin(GL_QUAD_STRIP);
	//Quads 1, 2, 3 y 4
	glTexCoord2f(0.0, 1.0); glVertex3f(1.0, -0.5, -3.0);  //V2
	glTexCoord2f(0.0, 0.0); glVertex3f(1.0, -1.0, -3.0);  //V1
	glTexCoord2f(1.0, 1.0); glVertex3f(1.0, -0.5, -3.5);  //V4
	glTexCoord2f(1.0, 0.0); glVertex3f(1.0, -1.0, -3.5);  //V3
	glTexCoord2f(2.0, 1.0); glVertex3f(-1.0, -0.5, -3.5);  //V6
	glTexCoord2f(2.0, 0.0); glVertex3f(-1.0, -1.0, -3.5);  //V5
	glTexCoord2f(3.0, 1.0); glVertex3f(-1.0, -0.5, -3.0);  //V8
	glTexCoord2f(3.0, 0.0); glVertex3f(-1.0, -1.0, -3.0);  //V7
	glTexCoord2f(4.0, 1.0); glVertex3f(1.0, -0.5, -3.0);  //V2
	glTexCoord2f(4.0, 0.0); glVertex3f(1.0, -1.0, -3.0);  //V1
	glEnd();
	glBegin(GL_QUADS);
	//Quad 5 (Cubierta)
	glTexCoord2f(0.0, 1.0); glVertex3f(-1.0, -0.5, -3.5); //V6
	glTexCoord2f(0.0, 0.0); glVertex3f(-1.0, -0.5, -3.0); //V8
	glTexCoord2f(1.0, 0.0); glVertex3f(1.0, -0.5, -3.0); //V2
	glTexCoord2f(1.0, 1.0); glVertex3f(1.0, -0.5, -3.5); //V4
	//Quad 6 (Cara inferior)
	glTexCoord2f(0.0, 1.0); glVertex3f(-1.0, -1.0, -3.0); //V7
	glTexCoord2f(0.0, 0.0); glVertex3f(-1.0, -1.0, -3.5); //V5
	glTexCoord2f(1.0, 0.0); glVertex3f(1.0, -1.0, -3.5); //V3
	glTexCoord2f(1.0, 1.0); glVertex3f(1.0, -1.0, -3.0); //V1
	glEnd();		
			
	//LARGUERO LATERAL IZQUIERDO
	glBegin(GL_QUAD_STRIP);
	//Quads 1, 2, 3 y 4
	glTexCoord2f(0.0, 1.0); glVertex3f(-1.0, -0.5, 3.0);  //V2
	glTexCoord2f(0.0, 0.0); glVertex3f(-1.0, -1.0, 3.0);  //V1
	glTexCoord2f(1.0, 1.0); glVertex3f(-1.0, -0.5, -3.0);  //V4
	glTexCoord2f(1.0, 0.0); glVertex3f(-1.0, -1.0, -3.0);  //V3
	glTexCoord2f(2.0, 1.0); glVertex3f(-1.5, -0.5, -3.0);  //V6
	glTexCoord2f(2.0, 0.0); glVertex3f(-1.5, -1.0, -3.0);  //V5
	glTexCoord2f(3.0, 1.0); glVertex3f(-1.5, -0.5, 3.0);  //V8
	glTexCoord2f(3.0, 0.0); glVertex3f(-1.5, -1.0, 3.0);  //V7
	glTexCoord2f(4.0, 1.0); glVertex3f(-1.0, -0.5, 3.0);  //V2
	glTexCoord2f(4.0, 0.0); glVertex3f(-1.0, -1.0, 3.0);  //V1
	glEnd();
	glBegin(GL_QUADS);
	//Quad 5 (Cubierta)
	glTexCoord2f(0.0, 1.0); glVertex3f(-1.5, -0.5, -3.0); //V6
	glTexCoord2f(0.0, 0.0); glVertex3f(-1.5, -0.5, 3.0); //V8
	glTexCoord2f(1.0, 0.0); glVertex3f(-1.0, -0.5, 3.0); //V2
	glTexCoord2f(1.0, 1.0); glVertex3f(-1.0, -0.5, -3.0); //V4
	//Quad 6 (Cara inferior)
	glTexCoord2f(0.0, 1.0); glVertex3f(-1.5, -1.0, 3.0); //V7
	glTexCoord2f(0.0, 0.0); glVertex3f(-1.5, -1.0, -3.0); //V5
	glTexCoord2f(1.0, 0.0); glVertex3f(-1.0, -1.0, -3.0); //V3
	glTexCoord2f(1.0, 1.0); glVertex3f(-1.0, -1.0, 3.0); //V1
	glEnd();					
	
	//LARGUERO LATERAL DERECHO
	glBegin(GL_QUAD_STRIP);
	//Quads 1, 2, 3 y 4
	glTexCoord2f(0.0, 1.0); glVertex3f(1.5, -0.5, 3.0);  //V2
	glTexCoord2f(0.0, 0.0); glVertex3f(1.5, -1.0, 3.0);  //V1
	glTexCoord2f(1.0, 1.0); glVertex3f(1.5, -0.5, -3.0);  //V4
	glTexCoord2f(1.0, 0.0); glVertex3f(1.5, -1.0, -3.0);  //V3
	glTexCoord2f(2.0, 1.0); glVertex3f(1.0, -0.5, -3.0);  //V6
	glTexCoord2f(2.0, 0.0); glVertex3f(1.0, -1.0, -3.0);  //V5
	glTexCoord2f(3.0, 1.0); glVertex3f(1.0, -0.5, 3.0);  //V8
	glTexCoord2f(3.0, 0.0); glVertex3f(1.0, -1.0, 3.0);  //V7
	glTexCoord2f(4.0, 1.0); glVertex3f(1.5, -0.5, 3.0);  //V2
	glTexCoord2f(4.0, 0.0); glVertex3f(1.5, -1.0, 3.0);  //V1
	glEnd();
	
	glBegin(GL_QUADS);
	//Quad 5 (Cubierta)
	glTexCoord2f(0.0, 1.0); glVertex3f(1.0, -0.5, -3.0); //V6
	glTexCoord2f(0.0, 0.0); glVertex3f(1.0, -0.5, 3.0); //V8
	glTexCoord2f(1.0, 0.0); glVertex3f(1.5, -0.5, 3.0); //V2
	glTexCoord2f(1.0, 1.0); glVertex3f(1.5, -0.5, -3.0); //V4
	//Quad 6 (Cara inferior)
	glTexCoord2f(0.0, 1.0); glVertex3f(1.0, -1.0, 3.0); //V7
	glTexCoord2f(0.0, 0.0); glVertex3f(1.0, -1.0, -3.0); //V5
	glTexCoord2f(1.0, 0.0); glVertex3f(1.5, -1.0, -3.0); //V3
	glTexCoord2f(1.0, 1.0); glVertex3f(1.5, -1.0, 3.0); //V1
	glEnd();	
	glPopAttrib();
}

void initLights()
{
  glEnable(GL_NORMALIZE);
 
  glEnable(GL_LIGHT0);
  static const GLfloat lightPos[4] = { 0.0f, 9.98f, 0.0f, 1.0f };
  GLfloat ambient_light[] = { 0.1, 0.1, 0.1, 1.0 };
  glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
  glLightfv(GL_LIGHT0, GL_AMBIENT,ambient_light);

  }

void setTheMaterial(const GLfloat *ambient, const GLfloat *diffuse, const GLfloat *specular , const GLfloat shininess )
{
  glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
  //glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
  //glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
  //glMaterialf(GL_FRONT, GL_SHININESS, shininess);
}
void initScene()
{
	glShadeModel(GL_SMOOTH);                    // shading mathod: GL_SMOOTH or GL_FLAT
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);      // 4-byte pixel alignment

    // enable /disable features
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    
    glEnable(GL_LIGHTING);
    glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
     // track material ambient and diffuse from surface color, call it before glEnable(GL_COLOR_MATERIAL)
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);

    glClearColor(0, 0, 0, 0);                   // background color
    glClearStencil(0);                          // clear stencil buffer
    glClearDepth(1.0f);                         // 0 is near, 1 is far
    glDepthFunc(GL_LEQUAL);
		
	initLights();
	initTexture();
}

void drawScene()
{
	
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  glGetFloatv(GL_PROJECTION_MATRIX, projMatrix);
  glGetFloatv(GL_MODELVIEW_MATRIX, viewMatrix);
  
  
  glPushMatrix();
  glColor3f(1.0,1.0,1.0);
  glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  setTheMaterial(blanco,blanco,blanco,100);
  glTranslatef(0.0,-0.05,0.0);
  glScalef(0.2,1.0,0.2);
  glBegin(GL_QUADS);
  glVertex3fv(v1);
  glVertex3fv(v6);
  glVertex3fv(v5);
  glVertex3fv(v2);
  glEnd();
  
  glPopAttrib();
  glPopMatrix();
  
  glEnable(GL_TEXTURE_2D);
  //PISO
  glBindTexture(GL_TEXTURE_2D, textures[0]);
  glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glColor3f(1.0,1.0,1.0);
  glBegin(GL_QUADS);
  glTexCoord2f(1.0,0.0); glVertex3fv(v4);
  glTexCoord2f(1.0,1.0); glVertex3fv(v3);
  glTexCoord2f(0.0,1.0); glVertex3fv(v8);
  glTexCoord2f(0.0,0.0); glVertex3fv(v7);
  glEnd();     
  glPopAttrib();
   
  //Arriba
  glBindTexture(GL_TEXTURE_2D, textures[2]);
  glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glColor3f(1.0,1.0,1.0);
  glBegin(GL_QUADS);
  glTexCoord2f(0.0,0.0); glVertex3fv(v1);
  glTexCoord2f(1.0,0.0); glVertex3fv(v6);
  glTexCoord2f(1.0,1.0); glVertex3fv(v5);
  glTexCoord2f(0.0,1.0); glVertex3fv(v2);
  glEnd();
  glPopAttrib();
  //textura paredes
  //Atras
  glBindTexture(GL_TEXTURE_2D, textures[1]);
  glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glColor3f(1.0,1.0,1.0);
  glBegin(GL_QUADS);
  glTexCoord2f(1.0,1.0); glVertex3fv(v2);
  glTexCoord2f(0.0,1.0); glVertex3fv(v5);
  glTexCoord2f(0.0,0.0); glVertex3fv(v8);
  glTexCoord2f(1.0,0.0); glVertex3fv(v3);
  glEnd();
  //Adelante
  glBegin(GL_QUADS);
  glTexCoord2f(0.0,1.0); glVertex3fv(v1);
  glTexCoord2f(0.0,0.0); glVertex3fv(v4);
  glTexCoord2f(1.0,0.0); glVertex3fv(v7);
  glTexCoord2f(1.0,1.0); glVertex3fv(v6); 
  glEnd();  
  //Derecha
  glBegin(GL_QUADS);
  glTexCoord2f(1.0,1.0); glVertex3fv(v1);
  glTexCoord2f(0.0,1.0); glVertex3fv(v2);
  glTexCoord2f(0.0,0.0); glVertex3fv(v3);
  glTexCoord2f(1.0,0.0); glVertex3fv(v4);
  glEnd();
  //Izquierda
  glBegin(GL_QUADS);
  glTexCoord2f(0.0,0.0); glVertex3fv(v7);
  glTexCoord2f(1.0,0.0); glVertex3fv(v8);
  glTexCoord2f(1.0,1.0); glVertex3fv(v5);
  glTexCoord2f(0.0,1.0); glVertex3fv(v6);
  glEnd();
  glPopAttrib();
  
  glPushMatrix();
  glTranslatef(-7.0f, 3.5f, 0.0f);
  glScalef(1.0f, 1.0f, 0.5f); 
  table();
  glPopMatrix();
  
  glPushMatrix();
  glTranslatef(7.0f, 3.5f, -1.0f);
  glScalef(1.0f, 1.0f, 0.5f); 
  table();
  glPopMatrix();  
  
  glDisable(GL_TEXTURE_2D);
  
  glPushMatrix();
    glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glColor3f(1.0,0.0,1.0);
   glTranslatef(7.0f,4.0f,-1.0f);
   glutSolidSphere(0.5,30,30);
   glPopAttrib();
  glPopMatrix();
  
   glPushMatrix();
     glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glColor3f(0.0,1.0,1.0);
    glTranslatef(-7.0f,4.0f,0.0f);
    glutSolidSphere(0.5,30,30);
    glPopAttrib();
   glPopMatrix();
}

void idle()
{
  if (angle > 360.0)
    angle = 0;
  angle = angle + 0.5;

  glutPostRedisplay();
}

void renderToTexture()
{
	glBindFramebuffer(GL_FRAMEBUFFER, fboID);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glPushMatrix();
	glViewport(0,0,TEXTURE_WIDTH,TEXTURE_HEIGHT);
	glMatrixMode(GL_PROJECTION);
	
	glLoadIdentity();
	gluPerspective(90.0f, (float)(TEXTURE_WIDTH)/TEXTURE_HEIGHT, 1.0f, 100.0f);
	gluLookAt(0.0f,5.0f,-eyeZ,
	          dirX,dirY,dirZ,
	          0.0f,1.0f,0.0f);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	drawScene();
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0); // unbind

        // trigger mipmaps generation explicitly
        // NOTE: If GL_GENERATE_MIPMAP is set to GL_TRUE, then glCopyTexSubImage2D()
        // triggers mipmap generation automatically. However, the texture attached
        // onto a FBO should generate mipmaps manually via glGenerateMipmap().
    glBindTexture(GL_TEXTURE_2D, texture4Render);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
	//glPopMatrix();
}
void renderToWindow()
{
 
  glPolygonMode(GL_FRONT, GL_FILL);
  glPolygonMode(GL_BACK, GL_LINE);
  
  renderToTexture();

  glViewport(0, 0, screen_width, screen_height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  if(screen_height <= 0)
    screen_height = 1.0;
  gluPerspective(100.0,screen_width/screen_height,1,30.0);
  gluLookAt(eyeX,eyeY,eyeZ, //eye
	        dirX,dirY,dirZ,  //where
	       0.0,1.0,0.0); //up
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  
  glPushMatrix();
  drawScene();
 glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texture4Render);
  glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glTranslatef(0.0,2.5,-4.9);
  glScalef(0.5,0.5,0.5);
  glBegin(GL_QUADS);
  glColor3f(1.0,1.0,1.0);
  glTexCoord2f(0.0,1.0); glVertex3fv(v2);
  glTexCoord2f(1.0,1.0); glVertex3fv(v5);
  glTexCoord2f(1.0,0.0); glVertex3fv(v8);
  glTexCoord2f(0.0,0.0); glVertex3fv(v3);
  glEnd();
  glPopAttrib();
  glPopMatrix();
  glDisable(GL_TEXTURE_2D);
  
  glutSwapBuffers();
  
 
}

void reshape(int w, int h)
{

 screen_width = w;
 screen_height = h;
 
}

void keyboard(unsigned char key, int x, int y)
{
  switch(key)
    {
    case 27:
      exit(0);
      break;
    case 's':
      break;
    case 'l':
      glDisable(GL_LIGHTING);
      break;      
    case 'h':
      glEnable(GL_LIGHTING);
      break;    
    case 'e':
	  eyeZ++;	
	  break;
	case 'E':
	  eyeZ--;	
	  break;  
    case 'p':
      break;
    }
  glutPostRedisplay();
}

void clearSharedMem()
{
    glDeleteTextures(1, &texture4Render);
    texture4Render = 0;
	glDeleteTextures(5, textures);
	for(int i = 0; i < numTextures; i++)
	{
		textures[i] = 0;
	}

    glDeleteFramebuffers(1, &fboID);
    fboID = 0;
    glDeleteRenderbuffers(1, &rboID);
    rboID = 0;
}


int main(int argc, char **argv)
{
	
 
  glutInit(&argc, argv);
  
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowPosition(50, 50);
  glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
  glutCreateWindow("Modelo Iluminacion");

 GLenum err = glewInit();
  if (GLEW_OK != err)
    {

      cout << "Error: " << glewGetErrorString(err) << endl;
    }


  cout << "Status: Using GLEW " << glewGetString(GLEW_VERSION) << endl;


  glInfo info;
  info.getInfo();
  //info.printSelf();

  if(info.isExtensionSupported("GL_EXT_framebuffer_object"))
    {
      fboSupported = fboUsed = true;
      cout << "Video card supports GL_EXT_framebuffer_object." << endl;
    }
  else
    {
      fboSupported = fboUsed = false;
      cout << "Video card does NOT support GL_EXT_framebuffer_object." << endl;
    }
    
  initScene();
  glutReshapeFunc(reshape);  
  glutDisplayFunc(renderToWindow);
  glutIdleFunc(idle);
  glutKeyboardFunc(keyboard);
  glutMainLoop();
  
  clearSharedMem();
  
  return 0;
}

