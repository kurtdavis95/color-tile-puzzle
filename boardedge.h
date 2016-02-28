/***************\
* Kurt Davis	 \
* 2/25/16	  \
* boardedge.h	   \
*********************************************************************************\
* Interface file for BoardEdge class. 						 *
* This class represents the 10-segment 1x4 frame. Each instance includes its own *
* 	OpenGL Vertex Array Object, with associated Vertex Buffers for color	 *
*	and position attributes.						 *
*										 *

 BoardEdge is an assembly of 10 trapezoids (20 triangles) of the following construction:
	 _________________________________________________
	|\_____1_____|_____2_____|_____3_____|_____4____ /|
	| |                                             | |
	|0|                                             |9|
	| |_____________________________________ _______| |
	|/_____5_____|_____6_____|_____7_____|_____8_____\|
	
 Where the composing polygons are indexed as shown above.

 This indexing convention is followed throughout lengthy array declarations
 to hold back an avalanche of redundant, incomprehensible comments at said locations.	

*										  *
\*********************************************************************************/

#ifndef BOARDEDGE_H
#define BOARDEDGE_H

//Required Libraries
//#define GLEW_STATIC
#include <GL/glew.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include "boardtile.h"	//needed for color enumeration
#include <string>

//********************************************************************************

class BoardEdge {
	public:
		BoardEdge(GLuint, std::string, GLfloat = 0, GLfloat = 0, int = 0);	//Default constructor
		~BoardEdge();			//Deconstructor

		int isVisible();		//returns value of visibility
		void changeVisibility();	//inverts value of visibility
		void resetColorData();		//refreshes data in color buffer to correspond with colors[]
		void render();
	private:
		GLuint vao;	//Tile's vertex array object
		GLuint vbo;	//Tile's vertex buffer
		GLuint vco;	//Tile's color data buffer

		GLuint shader;	//Shader Program Identifier

		int visibility;
		int colors[10];	//Tile's quadrant colors
}; 

#endif
