/****************\
* Kurt Davis	 *
* 2/25/16	 *
* boardtile.cpp	 *
\****************/

//#define GLEW_STATIC
#include <GL/glew.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include "boardtile.h"

#include <cstdio>
#include <string>

//Constructor
BoardTile::BoardTile(GLuint shaderId, std::string cset, GLfloat x, GLfloat y, int v)
{
	
	try{
		if(cset.length() < 4){
			throw 3;
		}	
	}catch (int e){
		printf("Exception %i thrown in BoardTile Constructor;\nProbably color code string length < 4.", e);
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
		x-0.16f, y+0.2f,    //top-left
		x+0.16f, y+0.2f,     //top-right
		x+0.0f, y+0.0f,     //center

		x+0.16f, y+0.2f,     //top-right
		x+0.16f, y-0.2f,    //bottom-right
		x+0.0f, y+0.0f,     //center

		x+0.16f, y-0.2f,    //bottom-right
		x-0.16f, y-0.2f,   //bottom-left
		x+0.0f, y+0.0f,     //center

		x-0.16f, y-0.2f,   //bottom-left
		x-0.16f, y+0.2f,    //top-left
		x+0.0f, y+0.0f      //center
	};

	int i;
	for(i = 0; i < 4; i++){
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
BoardTile::~BoardTile()
{
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glDeleteBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vco);
	glDeleteBuffers(1, &vco);
	glDeleteVertexArrays(1, &vao);
}

//Copy Constructor
BoardTile::BoardTile(const BoardTile& other)
{
	GLint size = 0;

	shader = other.shader;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glGenBuffers(1, &vco);
	
	glBindBuffer(GL_COPY_READ_BUFFER, other.vbo);
	glGetBufferParameteriv(GL_COPY_READ_BUFFER, GL_BUFFER_SIZE, &size);

	glBindBuffer(GL_COPY_WRITE_BUFFER, vbo);
	glBufferData(GL_COPY_WRITE_BUFFER, size, nullptr, GL_STATIC_DRAW);

	glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, size);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	GLuint posAttrib = glGetAttribLocation(shader, "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);

	
	size = 0;		

	glBindBuffer(GL_COPY_READ_BUFFER, other.vco);
	glGetBufferParameteriv(GL_COPY_READ_BUFFER, GL_BUFFER_SIZE, &size);

	glBindBuffer(GL_COPY_WRITE_BUFFER, vco);
	glBufferData(GL_COPY_WRITE_BUFFER, size, nullptr, GL_STATIC_DRAW);

	glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, size);
	
	glBindBuffer(GL_ARRAY_BUFFER, vco);
	GLuint colorAttrib = glGetAttribLocation(shader, "color");
	glEnableVertexAttribArray(colorAttrib);
	glVertexAttribPointer(colorAttrib, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	glBindBuffer(GL_COPY_READ_BUFFER, 0);
	glBindBuffer(GL_COPY_WRITE_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	this->visibility = other.visibility;
	for(int i = 0; i < 4; i++){
		this->colors[i] = other.colors[i];
	}

	resetColorData();

}

//Assignment Operator
BoardTile& BoardTile::operator=(const BoardTile& other)
{
	if(this == &other){
		return *this;
	}else{	

		glDeleteBuffers(1, &vao);
		glDeleteBuffers(1, &vco);
		glDeleteVertexArrays(1, &vao);

		GLint size = 0;

		shader = other.shader;

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glGenBuffers(1, &vbo);
		glGenBuffers(1, &vco);
		
		glBindBuffer(GL_COPY_READ_BUFFER, other.vbo);
		glGetBufferParameteriv(GL_COPY_READ_BUFFER, GL_BUFFER_SIZE, &size);

		glBindBuffer(GL_COPY_WRITE_BUFFER, vbo);
		glBufferData(GL_COPY_WRITE_BUFFER, size, nullptr, GL_STATIC_DRAW);

		glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, size);
		
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		GLuint posAttrib = glGetAttribLocation(shader, "position");
		glEnableVertexAttribArray(posAttrib);
		glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);

		
		size = 0;		

		glBindBuffer(GL_COPY_READ_BUFFER, other.vco);
		glGetBufferParameteriv(GL_COPY_READ_BUFFER, GL_BUFFER_SIZE, &size);

		glBindBuffer(GL_COPY_WRITE_BUFFER, vco);
		glBufferData(GL_COPY_WRITE_BUFFER, size, nullptr, GL_STATIC_DRAW);

		glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, size);
		
		glBindBuffer(GL_ARRAY_BUFFER, vco);
		GLuint colorAttrib = glGetAttribLocation(shader, "color");
		glEnableVertexAttribArray(colorAttrib);
		glVertexAttribPointer(colorAttrib, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

		glBindBuffer(GL_COPY_READ_BUFFER, 0);
		glBindBuffer(GL_COPY_WRITE_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		this->visibility = other.visibility;
		for(int i = 0; i < 4; i++){
			this->colors[i] = other.colors[i];
		}

		resetColorData();
		
		return *this;
	}
}

//Returns value of visibility
int BoardTile::isVisible()
{
	return visibility;
}

//Invert value of Visibility
void BoardTile::changeVisibility()
{
	visibility = 1-visibility;
	resetColorData();
}

//Refresh data in OpenGL Color buffer
void BoardTile::resetColorData()
{
	glBindVertexArray(vao);
	
	int i;	

	//Initialize Color Data
	GLfloat cquad[4][3];
	if(isVisible()){	//send regular color data
		for(i = 0; i < 4; i++){
			switch(colors[i]){
				case CRED:
					cquad[i][0] = 1.0f;
					cquad[i][1] = 0.0f;
					cquad[i][2] = 0.0f;
					break;

				case CGREEN:
					cquad[i][0] = 0.0f;
					cquad[i][1] = 1.0f;
					cquad[i][2] = 0.0f;
					break;
				case CBLUE:	
					cquad[i][0] = 0.0f;
					cquad[i][1] = 0.0f;
					cquad[i][2] = 1.0f;
					break;			
				case CYELLOW:
					cquad[i][0] = 1.0f;
					cquad[i][1] = 1.0f;
					cquad[i][2] = 0.0f;
					break;
				case CBLACK:
					cquad[i][0] = 0.0f;
					cquad[i][1] = 0.0f;
					cquad[i][2] = 0.0f;
				default:
					//lolwut
					break;
			}
		}
	}else{			//send all vertex colors as black (modify with background color)
		for(i = 0; i < 4; i++){
			cquad[i][0] = 0.0f;
			cquad[i][1] = 0.0f;
			cquad[i][2] = 0.0f;
		}
	}

	//expand color data for entire triangles
	GLfloat colors[] = {
		cquad[0][0], cquad[0][1], cquad[0][2],
		cquad[0][0], cquad[0][1], cquad[0][2],
		cquad[0][0], cquad[0][1], cquad[0][2],

		cquad[1][0], cquad[1][1], cquad[1][2],
		cquad[1][0], cquad[1][1], cquad[1][2],
		cquad[1][0], cquad[1][1], cquad[1][2],

		cquad[2][0], cquad[2][1], cquad[2][2],
		cquad[2][0], cquad[2][1], cquad[2][2],
		cquad[2][0], cquad[2][1], cquad[2][2],

		cquad[3][0], cquad[3][1], cquad[3][2],
		cquad[3][0], cquad[3][1], cquad[3][2],
		cquad[3][0], cquad[3][1], cquad[3][2]
	};

	//Add color data to color buffer
	glBindBuffer(GL_ARRAY_BUFFER, vco);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

}

//Draw buffer contents
void BoardTile::render()
{
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 12);
	glBindVertexArray(0);
}

//Rotate color values
void BoardTile::rotate(int direction)
{
	int temp;
	if(direction){	//rotate clockwise
		temp = colors[0];
		colors[0] = colors[3];
		colors[3] = colors[2];
		colors[2] = colors[1];
		colors[1] = temp;
	}else{
		temp = colors[0];
		colors[0] = colors[1];
		colors[1] = colors[2];
		colors[2] = colors[3];
		colors[3] = temp;
	}
	resetColorData();
}

//Update color array via Vec4 input (for copying color data)
void BoardTile::setColors(Vec4 c)
{
	for(int i = 0; i < 4; i++){
		colors[i] = c.x[i];
	} 
	resetColorData();
}
