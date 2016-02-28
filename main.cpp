/**************\
* Kurt Davis    \
* 2/25/16	 \
* main.cpp	  \
********************************************************************************************\
* Driver file for simple 4-tile puzzle. Originally created as Theory of Computing  	    *
* Course Project 1. This file of the program:						    *
*	- Creates an OpenGL instance using SDL2						    *
*	- Compiles, links, and activates simple GLSL Shaders (hard coded) to		    *
*		build full OpenGL rendering pipeline.					    *
*	- Instantiates Game Objects: BoardEdge, BoardTile(x8), and Board Selector.	    *
*		For more information on each, see their respective .h and .cpp files.	    *
*	- Runs core game loop, including render calls, I/O handling, and game state logic.  *
*											    *
* Note that this project requires SDL2.0 and GLEW 1.13.0, with library PATH variables       *
*	configured appropriately.							    *
\*******************************************************************************************/

//Required Libraries
//#define GLEW_STATIC	//not needed with appropriate linker flag in Makefile
#include <GL/glew.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

//Other Project Headers
#include "boardtile.h"
#include "boardedge.h"
#include "boardselector.h"

//Additional Standard Libraries
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>

//Window Size Parameters
#define DEFAULT_WINDOW_HEIGHT 600
#define DEFAULT_WINDOW_WIDTH 800

//Puzzle Piece Color Parameters - See boardtile.h and boardedge.h for more details
#define TILESET0 "gbbb"
#define TILESET1 "gybb"
#define TILESET2 "brgy"
#define TILESET3 "brgr"

#define PUZZLEFRAME "bggbbbbggr"

//******************************************************************************************

//Global Variables

SDL_Window* window;	//handle for SDL2 window

GLuint vertexShader;	//vertex shader identifier
GLuint fragmentShader;	//fragment shader identifier
GLuint shaderProgramId;	//handle for shader program

//Vertex Shader Source Code
const GLchar* vertexSource =
	"#version 150 core\n"
	"in vec2 position;"
	"in vec3 color;"
	"out vec3 Color;"
	"void main()"
	"{"
	"    Color = color;"
	"    gl_Position = vec4(position, 0.0, 1.0);"
	"}";

//Fragment Shader Source Code
const GLchar* fragmentSource = 
	"#version 150 core\n"
	"in vec3 Color;"
	"out vec4 outColor;"
	"void main()"
	"{"
	"    outColor = vec4(Color, 1.0);"
	"}";

//Start main function
int main(int argc, char *argv[])
{
	//Initialize SDL2 Video module for OpenGL Context
	SDL_Init(SDL_INIT_VIDEO);
	
	//Specify OpenGL v3.2 Context
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

	//Create SDL2 window
	window = SDL_CreateWindow("Puzzle", 100, 100, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, SDL_WINDOW_OPENGL);

	//Create OpenGL context
	SDL_GLContext context = SDL_GL_CreateContext(window);


	GLenum err;	//Variable to hold error return values

	//Initialize GLEW
	glewExperimental = GL_TRUE;
	glewInit();

	glGetError();	//Reset error enum
	//Error checking block, repeated with output including error location.	
	while((err = glGetError()) != GL_NO_ERROR){
		std::cout << "OpenGL error @ 1: " << err << std::endl;
	}

	//Create and compile vertex shader
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);
	while((err = glGetError()) != GL_NO_ERROR){
		std::cout << "OpenGL error @ 2: " << err << std::endl;
	}

	//Create and compile fragement shader
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);
	while((err = glGetError()) != GL_NO_ERROR){
		std::cout << "OpenGL error @ 3: " << err << std::endl;
	}

	//Create program and link shaders
	shaderProgramId = glCreateProgram();
	glAttachShader(shaderProgramId, vertexShader);
	glAttachShader(shaderProgramId, fragmentShader);
	glBindFragDataLocation(shaderProgramId, 0, "outColor");
	glLinkProgram(shaderProgramId);
	glUseProgram(shaderProgramId);
	while((err = glGetError()) != GL_NO_ERROR){
		std::cout << "OpenGL error @ 4: " << err << std::endl;
	}

	//Polulate tileset
	std::string blanktile = "yyyy";
	std::vector<std::string> tileset;	//container holding strings representing tile quadrant colors
	tileset.push_back(TILESET0);
	tileset.push_back(TILESET1);
	tileset.push_back(TILESET2);
	tileset.push_back(TILESET3);

	//Container for top row of tiles, initialized to be invisible
	std::vector<BoardTile> top;
	for(int i = 0; i < 4; i++){
		top.push_back(BoardTile(shaderProgramId, tileset[i], (-0.48 + (0.32 * i)), 0.4, 0));
	}

	//Container for bottom row to tiles, initialized to be visible; colors and rotations should
	//	not be modified.
	std::vector<BoardTile> bottom;
	for(int i = 0; i < 4; i++){
		bottom.push_back(BoardTile(shaderProgramId, tileset[i], (-0.60 + (0.4 * i)), -0.4, 1));
	}

	//Instantiate puzzle frame.
	//Note that frame and selector do not have copy control yet (TODO).
	//If you fiddle with copies or assignments, GPU memory will blow up in your face.
	std::string becolors = PUZZLEFRAME;	//string encoding of edge colors
	BoardEdge b(shaderProgramId, becolors, -0.72, 0.68, 1);

	//Instantiate selector (grey box indicating game state).
	BoardSelector s(shaderProgramId);
	
	//Arrays of X and Y coordinate pairs for each selector state (location).
	GLfloat sPosX[] = {
		-0.53f, -0.21f, 0.11f, 0.43f,
		-0.65f, -0.25f, 0.15f, 0.55f
	};
	GLfloat sPosY[] = {
		0.45f, 0.45f, 0.45f, 0.45f,
		-0.36f, -0.36f, -0.36f, -0.36f
	};
	
	//Set selector to state 1 and update it
	int sState = 1;
	s.resetPositionData(sPosX[sState], sPosY[sState]);

	//Final initialization error check
	while((err = glGetError()) != GL_NO_ERROR){
		std::cout << "OpenGL error @ 5: " << err << std::endl;
	}


	SDL_Event windowEvent;	//I/O event for handling
	//MAIN GAME LOOP
	while (true)
	{
		//****************************Event Handling************************************
		if(SDL_PollEvent(&windowEvent))
		{
			
			//----------------Exit conditions-------------------
			if(windowEvent.type == SDL_QUIT) break;
			if(windowEvent.type == SDL_KEYUP && windowEvent.key.keysym.sym == SDLK_ESCAPE) break;	
			
			//----------------Tile Assignment-------------------
			if(windowEvent.type == SDL_KEYUP && windowEvent.key.keysym.sym == SDLK_1){
				if(sState > 3){		//Assign to space 1 (top left) on NUM1 press
					top[0].setColors(bottom[sState-4].getColors());
					if(!top[0].isVisible())
						top[0].changeVisibility();
					top[0].resetColorData();
				}
			}
			if(windowEvent.type == SDL_KEYUP && windowEvent.key.keysym.sym == SDLK_2){
				if(sState > 3){		//Assign to space 2 (top middle-left) on NUM2 press
					top[1].setColors(bottom[sState-4].getColors());
					if(!top[1].isVisible())
						top[1].changeVisibility();
					top[1].resetColorData();
				}
			}
			if(windowEvent.type == SDL_KEYUP && windowEvent.key.keysym.sym == SDLK_3){
				if(sState > 3){		//Assign to space 3 (top middle-right) on NUM3 press
					top[2].setColors(bottom[sState-4].getColors());
					if(!top[2].isVisible())
						top[2].changeVisibility();
					top[2].resetColorData();
				}
			}
			if(windowEvent.type == SDL_KEYUP && windowEvent.key.keysym.sym == SDLK_4){
				if(sState > 3){		//Assign to space 4 (top right) on NUM4 press
					top[3].setColors(bottom[sState-4].getColors());
					if(!top[3].isVisible())
						top[3].changeVisibility();
					top[3].resetColorData();
				}
			}
			//----------------Rotation and Removal--------------
			if(windowEvent.type == SDL_KEYDOWN && windowEvent.key.keysym.sym == SDLK_e){
				if(sState < 4){		//Rotate clockwise on E press
					top[sState].rotate(1);
				}
			}
			if(windowEvent.type == SDL_KEYDOWN && windowEvent.key.keysym.sym == SDLK_w){
				if(sState < 4){		//Remove on W press
					if(top[sState].isVisible())
						top[sState].changeVisibility();
				}
			}
			if(windowEvent.type == SDL_KEYDOWN && windowEvent.key.keysym.sym == SDLK_q){
				if(sState < 4){		//Rotate counter-clockwise on Q press
					top[sState].rotate(0);
				}
			}
	
			//----------------SELECTOR MOVEMENT-----------------
			if(windowEvent.type == SDL_KEYDOWN && windowEvent.key.keysym.sym == SDLK_UP){
				if(sState > 3)	//Move UP on UP press
					sState -= 4;
				s.resetPositionData(sPosX[sState], sPosY[sState]);
			}
			if(windowEvent.type == SDL_KEYDOWN && windowEvent.key.keysym.sym == SDLK_DOWN){
				if(sState < 4)	//Move DOWN on DOWN press
					sState += 4;
				s.resetPositionData(sPosX[sState], sPosY[sState]);
			}
			if(windowEvent.type == SDL_KEYDOWN && windowEvent.key.keysym.sym == SDLK_LEFT){
				if((sState % 4) != 0)	//Move LEFT on LEFT press
					sState -= 1;
				s.resetPositionData(sPosX[sState], sPosY[sState]);
			}
			if(windowEvent.type == SDL_KEYDOWN && windowEvent.key.keysym.sym == SDLK_RIGHT){
				if((sState % 4) != 3)	//Move RIGHT on RIGHT press
					sState += 1;
				s.resetPositionData(sPosX[sState], sPosY[sState]);
			}
		}

		//Set background to black and clear framebuffer
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Render each object
		for(int i = 0; i < 4; i++){
			if(top[i].isVisible())
				top[i].render();
			if(bottom[i].isVisible())
				bottom[i].render();
		}

		b.render();
		s.render();
		
		//Swap framebuffers
		SDL_GL_SwapWindow(window);
	}

	//Delete shaders
	glDeleteProgram(shaderProgramId);
	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);

	//Destroy OpenGL context
	SDL_GL_DeleteContext(context);
	
	//Clean up SDL2 and exit	
	SDL_Quit();
	return 0;
}
