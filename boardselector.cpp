/************************\
* Kurt Davis	 	 *
* 2/25/16	 	 *
* boardselector.cpp	 *
\************************/
//#define GLEW_STATIC
#include <GL/glew.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include "boardselector.h"

#include <cstdio>
#include <string>

//Constructor
BoardSelector::BoardSelector(GLuint shaderId, GLfloat x, GLfloat y)
{
	shader = shaderId;

	//Generate Vertex Array Object
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//Generate Vertex Buffer Object
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &vco);

	//Temporary Vertex Data
	GLfloat vertices[] = {
		x, y,    	//top-left
		x+0.1f, y,     	//top-right
		x, y-0.1f,     	//bottom-left

		x, y-0.1f,     	//bottom-left
		x+0.1f, y-0.1f,	//bottom-right
		x+0.1f, y    	//top-right
	};
	
	//Selector painted grey
	GLfloat colors[] = {
		0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f,
	
		0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f
	};

	//Add vertex data to vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Add color data to color buffer
	glBindBuffer(GL_ARRAY_BUFFER, vco);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

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
BoardSelector::~BoardSelector()
{
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glDeleteBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vco);
	glDeleteBuffers(1, &vco);
	glDeleteVertexArrays(1, &vao);
}

//TODO: NO COPY CONTROL

//Update vertex posiion buffer
void BoardSelector::resetPositionData(GLfloat x, GLfloat y)
{
	glBindVertexArray(vao);

	GLfloat vertices[] = {
		x, y,    	//top-left
		x+0.1f, y,     	//top-right
		x, y-0.1f,     	//bottom-left

		x, y-0.1f,     	//bottom-left
		x+0.1f, y-0.1f,	//bottom-right
		x+0.1f, y    	//top-right
	};

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

//Draw buffer contents to frambuffer
void BoardSelector::render()
{
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}
