/****************\
* Kurt Davis	 *
* 2/25/16	 *
* boardedge.cpp	 *
\****************/

//#define GLEW_STATIC
#include <GL/glew.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include "boardtile.h"
#include "boardedge.h"

#include <cstdio>
#include <string>

/*
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
*/

//Constructor
BoardEdge::BoardEdge(GLuint shaderId, std::string cset, GLfloat x, GLfloat y, int v)
{
	
	try{
		if(cset.length() < 4){
			throw 3;
		}	
	}catch (int e){
		printf("Exception %i thrown in BoardEdge Constructor;\nProbably color code string length < 4.", e);
	}

	if(v){
		visibility = 1;
	}else{
		visibility = 0;
	}

	shader = shaderId;

	//Generate Vertex Array Object
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//Generate Vertex Buffer Object
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &vco);

	//Temporary Vertex Data
	GLfloat vertices[] = {
		//Polygon 0
		x, y,			//top left
		x + 0.075f, y-0.075f,	//top right
		x, y - 0.551f,		//bottom left
		
		x + 0.075f, y-0.075f,	//top right
		x + 0.075f, y-0.483f,	//bottom right
		x, y - 0.551f,		//bottom left

		//Polygon 1
		x, y,			//top left
		x + 0.40f, y,		//top right
		x + 0.075f, y - 0.075f,	//bottom left

		x + 0.40f, y,		//top right
		x + 0.40f, y - 0.075f,	//bottom right
		x + 0.075f, y - 0.075f,	//bottom left
	
		//Polygon 2
		x + 0.4f, y,		//top left
		x + 0.72f, y,		//top right
		x + 0.4f, y - 0.075f,	//bottom left

		x + 0.720f, y,		//top right
		x + 0.720f, y - 0.075f,	//bottom right
		x + 0.4f, y - 0.075f,	//bottom left

		//Polygon 3
		x + 0.72f, y,		//top left
		x + 1.04f, y,		//top right
		x + 0.72f, y - 0.075f,	//bottom left

		x + 1.04f, y,		//top right
		x + 1.04f, y - 0.075f,	//bottom right
		x + 0.72f, y - 0.075f,	//bottom left
		
		//Polygon 4
		x + 1.04f, y,		//top left
		x + 1.42f, y,		//top right
		x + 1.04f, y - 0.075f,	//bottom left

		x + 1.42f, y,		//top right
		x + 1.37f, y - 0.075f,	//bottom right
		x + 1.04f, y - 0.075f,	//bottom left

		//Polygon 5
		x + 0.075f, y-0.483f,	//top left
		x + 0.4f, y-0.483f,	//top right
		x, y - 0.551f,		//bottom left

		x + 0.4f, y-0.483f,	//top right
		x + 0.4f, y - 0.551f,	//bottom right
		x, y - 0.551f,		//bottom left

		//Polygon 6
		x + 0.4f, y-0.483f,	//top left
		x + 0.72f, y-0.483f,	//top right
		x + 0.4f, y - 0.551f,	//bottom left

		x + 0.72f, y-0.483f,	//top right
		x + 0.72f, y - 0.551f,	//bottom right
		x + 0.4f, y - 0.551f,	//bottom left

		//Polygon 7
		x + 0.72f, y-0.483f,	//top left
		x + 1.04f, y-0.483f,	//top right
		x + 0.72f, y - 0.551f,	//bottom left

		x + 1.04f, y-0.483f,	//top right
		x + 1.04f, y - 0.551f,	//bottom right
		x + 0.72f, y - 0.551f,	//bottom left
	
		//Polygon 8
		x + 1.04f, y-0.483f,	//top left
		x + 1.37f, y-0.483f,	//top right
		x + 1.04f, y - 0.551f,	//bottom left

		x + 1.37f, y-0.483f,	//top right
		x + 1.42f, y - 0.551f,	//bottom right
		x + 1.04f, y - 0.551f,	//bottom left

		//Polygon 9
		x + 1.37f, y - 0.075f,	//top left
		x + 1.42f, y,		//top right
		x + 1.37f, y - 0.483f,	//bottom left

		x + 1.42f, y,		//top right
		x + 1.42f, y - 0.551f,	//bottom right
		x + 1.37f, y - 0.483f	//bottom left
	};

	int i;
	for(i = 0; i < 10; i++){
		switch(cset[i]){
			case 'r':
				colors[i] = CRED;
				break;
			case 'g':
				colors[i] = CGREEN;
				break;
			case 'b':
				colors[i] = CBLUE;
				break;
			case 'y':
				colors[i] = CYELLOW;
				break;
			default:
				colors[i] = CBLACK;
				break;
		}
	}

	resetColorData();

	//Add vertex data to vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Specify buffer formatting
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	GLuint posAttrib = glGetAttribLocation(shader, "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);

	glBindBuffer(GL_ARRAY_BUFFER, vco);
	GLuint colAttrib = glGetAttribLocation(shader, "color");
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	//Unbind buffers and VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

//Deconstructor
BoardEdge::~BoardEdge()
{
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glDeleteBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vco);
	glDeleteBuffers(1, &vco);
	glDeleteVertexArrays(1, &vao);
}

//TODO: NO COPY CONTROL PRESENT

//Return value of visibility
int BoardEdge::isVisible()
{
	return visibility;
}

//Invert value of visibility
void BoardEdge::changeVisibility()
{
	visibility = 1-visibility;
	resetColorData();
}

//Update contents of OpenGL color buffer
void BoardEdge::resetColorData()
{
	glBindVertexArray(vao);
	
	int i;	

	//Initialize Color Data
	GLfloat cquad[20][3];
	if(isVisible()){	//send regular color data
		for(i = 0; i < 10; i++){
			switch(colors[i]){
				case CRED:
					cquad[(2*i)][0] = 1.0f;
					cquad[(2*i)][1] = 0.0f;
					cquad[(2*i)][2] = 0.0f;
					cquad[(2*i)+1][0] = 1.0f;
					cquad[(2*i)+1][1] = 0.0f;
					cquad[(2*i)+1][2] = 0.0f;
					break;

				case CGREEN:
					cquad[(2*i)][0] = 0.0f;
					cquad[(2*i)][1] = 1.0f;
					cquad[(2*i)][2] = 0.0f;
					cquad[(2*i)+1][0] = 0.0f;
					cquad[(2*i)+1][1] = 1.0f;
					cquad[(2*i)+1][2] = 0.0f;
					break;
				case CBLUE:
					cquad[(2*i)][0] = 0.0f;
					cquad[(2*i)][1] = 0.0f;
					cquad[(2*i)][2] = 1.0f;
					cquad[(2*i)+1][0] = 0.0f;
					cquad[(2*i)+1][1] = 0.0f;
					cquad[(2*i)+1][2] = 1.0f;
					break;			
				case CYELLOW:
					cquad[(2*i)][0] = 1.0f;
					cquad[(2*i)][1] = 1.0f;
					cquad[(2*i)][2] = 0.0f;
					cquad[(2*i)+1][0] = 1.0f;
					cquad[(2*i)+1][1] = 1.0f;
					cquad[(2*i)+1][2] = 0.0f;
					break;
				case CBLACK:
					cquad[(2*i)][0] = 0.0f;
					cquad[(2*i)][1] = 0.0f;
					cquad[(2*i)][2] = 0.0f;
					cquad[(2*i)+1][0] = 0.0f;
					cquad[(2*i)+1][1] = 0.0f;
					cquad[(2*i)+1][2] = 0.0f;
				default:
					//lolwut
					break;
			}
		}
	}else{			//send all vertex colors as black (modify with background color)
		for(i = 0; i < 10; i++){
			cquad[(2*i)][0] = 0.0f;
			cquad[(2*i)][1] = 0.0f;
			cquad[(2*i)][2] = 0.0f;
			cquad[(2*i)+1][0] = 0.0f;
			cquad[(2*i)+1][1] = 0.0f;
			cquad[(2*i)+1][2] = 0.0f;
		}
	}

	//TODO: Roll this into a loop; there is a smarter way to do this.
	//expand color data for entire triangles
	GLfloat colors[] = {
		//Polygon 0
		cquad[0][0], cquad[0][1], cquad[0][2],
		cquad[0][0], cquad[0][1], cquad[0][2],
		cquad[0][0], cquad[0][1], cquad[0][2],
		
		cquad[1][0], cquad[1][1], cquad[1][2],
		cquad[1][0], cquad[1][1], cquad[1][2],
		cquad[1][0], cquad[1][1], cquad[1][2],

		//Polygon 1
		cquad[2][0], cquad[2][1], cquad[2][2],
		cquad[2][0], cquad[2][1], cquad[2][2],
		cquad[2][0], cquad[2][1], cquad[2][2],

		cquad[3][0], cquad[3][1], cquad[3][2],
		cquad[3][0], cquad[3][1], cquad[3][2],
		cquad[3][0], cquad[3][1], cquad[3][2],

		//Polygon 2
		cquad[4][0], cquad[4][1], cquad[4][2],
		cquad[4][0], cquad[4][1], cquad[4][2],
		cquad[4][0], cquad[4][1], cquad[4][2],

		cquad[5][0], cquad[5][1], cquad[5][2],
		cquad[5][0], cquad[5][1], cquad[5][2],
		cquad[5][0], cquad[5][1], cquad[5][2],
		
		//Polygon 3
		cquad[6][0], cquad[6][1], cquad[6][2],
		cquad[6][0], cquad[6][1], cquad[6][2],
		cquad[6][0], cquad[6][1], cquad[6][2],

		cquad[7][0], cquad[7][1], cquad[7][2],
		cquad[7][0], cquad[7][1], cquad[7][2],
		cquad[7][0], cquad[7][1], cquad[7][2],
		
		//Polygon 4
		cquad[8][0], cquad[8][1], cquad[8][2],
		cquad[8][0], cquad[8][1], cquad[8][2],
		cquad[8][0], cquad[8][1], cquad[8][2],

		cquad[9][0], cquad[9][1], cquad[9][2],
		cquad[9][0], cquad[9][1], cquad[9][2],
		cquad[9][0], cquad[9][1], cquad[9][2],
	
		//Polygon 5
		cquad[10][0], cquad[10][1], cquad[10][2],
		cquad[10][0], cquad[10][1], cquad[10][2],
		cquad[10][0], cquad[10][1], cquad[10][2],

		cquad[11][0], cquad[11][1], cquad[11][2],
		cquad[11][0], cquad[11][1], cquad[11][2],
		cquad[11][0], cquad[11][1], cquad[11][2],

		//Polygon 6
		cquad[12][0], cquad[12][1], cquad[12][2],
		cquad[12][0], cquad[12][1], cquad[12][2],
		cquad[12][0], cquad[12][1], cquad[12][2],

		cquad[13][0], cquad[13][1], cquad[13][2],
		cquad[13][0], cquad[13][1], cquad[13][2],
		cquad[13][0], cquad[13][1], cquad[13][2],

		//Polygon 7
		cquad[14][0], cquad[14][1], cquad[14][2],
		cquad[14][0], cquad[14][1], cquad[14][2],
		cquad[14][0], cquad[14][1], cquad[14][2],

		cquad[15][0], cquad[15][1], cquad[15][2],
		cquad[15][0], cquad[15][1], cquad[15][2],
		cquad[15][0], cquad[15][1], cquad[15][2],

		//Polygon 8
		cquad[16][0], cquad[16][1], cquad[16][2],
		cquad[16][0], cquad[16][1], cquad[16][2],
		cquad[16][0], cquad[16][1], cquad[16][2],

		cquad[17][0], cquad[17][1], cquad[17][2],
		cquad[17][0], cquad[17][1], cquad[17][2],
		cquad[17][0], cquad[17][1], cquad[17][2],

		//Polygon 9
		cquad[18][0], cquad[18][1], cquad[18][2],
		cquad[18][0], cquad[18][1], cquad[18][2],
		cquad[18][0], cquad[18][1], cquad[18][2],

		cquad[19][0], cquad[19][1], cquad[19][2],
		cquad[19][0], cquad[19][1], cquad[19][2],
		cquad[19][0], cquad[19][1], cquad[19][2],
	};

	//Add color data to color buffer
	glBindBuffer(GL_ARRAY_BUFFER, vco);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

}

//Draw buffer contents to framebuffer
void BoardEdge::render()
{
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 60);
	glBindVertexArray(0);
}
