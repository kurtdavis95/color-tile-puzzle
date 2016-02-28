/***************\
* Kurt Davis	 \
* 2/25/16	  \
* boardtile.h	   \
*********************************************************************************\
* Interface file for BoardTile class. 						 *
* This class represents 4-quadrant tile. Each instance includes its own 	 *
* 	OpenGL Vertex Array Object, with associated Vertex Buffers for color	 *
*	and position attributes.						 *
*										 *
* ---Color encoding index layout---						 *
*   	 0									 *
*  3 	       1								 *
*	 2									 *
*										 *
\*********************************************************************************/

#ifndef BOARDTILE_H
#define BOARDTILE_H

//Required Libraries
//#define GLEW_STATIC
#include <GL/glew.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include <string>

//********************************************************************************

enum Color { CRED, CGREEN, CBLUE, CYELLOW, CBLACK };

struct Vec4{
	Vec4(int a, int b, int c, int d){
		x[0] = a;
		x[1] = b;
		x[2] = c;
		x[3] = d;
	}

	int x[4];
};

class BoardTile {
	public:
		BoardTile(GLuint, std::string, GLfloat = 0, GLfloat = 0, int = 0);	//Default constructor
		~BoardTile();			//Deconstructor
		BoardTile(const BoardTile &);	//Copy Constructor

		BoardTile& operator=(const BoardTile &);	//Assignment operator

		int isVisible();		//returns value of visibility
		void changeVisibility();	//inverts value of visibility
		void resetColorData();		//refreshes data in color buffer to correspond with colors[]
		void render();			//renders tile to screen
		void rotate(int);		//args = 0 -> rotate counterclockwise
						//args = 1 -> rotate clockwise
		void setColors(Vec4);		//get and set functions passing Vec4 object for color reassignment
		Vec4 getColors(){return Vec4(colors[0], colors[1], colors[2], colors[3]);};
	private:
		GLuint vao;	//Tile's vertex array object
		GLuint vbo;	//Tile's vertex buffer
		GLuint vco;	//Tile's color data buffer

		GLuint shader;	//Shader Program Identifier

		int visibility;
		int colors[4];	//Tile's quadrant colors
}; 

#endif
