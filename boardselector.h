/***************\
* Kurt Davis	 \
* 2/25/16	  \
* boardtile.h	   \
*********************************************************************************\
* Interface file for BoardSelector class. 					 *
* This class represents grey selection box. Each instance includes its own 	 *
* 	OpenGL Vertex Array Object, with associated Vertex Buffers for color	 *
*	and position attributes.						 *
*										 *
*										 *
\*********************************************************************************/

#ifndef BOARDSELECTOR_H
#define BOARDSELECTOR_H

//Required Libraries
//#define GLEW_STATIC
#include <GL/glew.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include <string>

class BoardSelector {
	public:
		BoardSelector(GLuint, GLfloat = 0, GLfloat = 0);	//Default constructor
		~BoardSelector();					//Deconstructor

		void resetPositionData(GLfloat = 0, GLfloat = 0);	//refreshes data in vertex buffer to correspond with position arguments
		void render();	
	private:
		GLuint vao;	//Selector's vertex array object
		GLuint vbo;	//Selector's vertex buffer
		GLuint vco;	//Selector's color data buffer

		GLuint shader;	//Shader Program Identifier
}; 

#endif
