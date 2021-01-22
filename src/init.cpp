#include "init.h"
#include <stdio.h>
#include <SDL2/SDL.h>
#include "glad/glad.h"
#include <SDL2/SDL_opengl.h>
#include <string.h>

/****************
    Variables
*****************/

// GL stuff
GLuint gProgramID = 0;
GLint gVertexPos2dLocation = -1;
GLuint VAO, VBO, myProgram;

SDL_Window *window;

// Event structure
SDL_Event e;

/********************************
    Shaders (move these later)
*********************************/

// Vertex Shader
static const char* vShader = "					\n\
#version 330									\n\
												\n\
layout (location = 0) in vec3 pos;				\n\
												\n\
void main(){									\n\
	gl_Position = vec4(pos, 1.0);			    \n\
}";

//Fragment Shader
static const char* fShader = "					\n\
#version 330									\n\
												\n\
out vec4 color;									\n\
                                                \n\
void main(){									\n\
	color = vec4( 1.0, 0.0, 0.0, 1.0);          \n\
}";

/***************************
    Function Definitions
****************************/

// Initialize OpenGL
void initOpenGL(){
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    // Create Context
    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    if(glContext == NULL){
        printf("Opengl context failed to initialize: %s\n", SDL_GetError());
        exit(-1);
    }

    // Get OpenGL function pointers
    gladLoadGLLoader(SDL_GL_GetProcAddress);

}

// Initialize SDL
void initSDL(){

    printf("Initializing SDL.\n");
    
    /* Initialize defaults, Video and Audio */

    if((SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO)==-1)) { 
        printf("Could not initialize SDL: %s.\n", SDL_GetError());
        exit(-1);
    }
    atexit(SDL_Quit);

    printf("SDL initialized.\n");

    window = SDL_CreateWindow("Test", WINDOWPOS_X, WINDOWPOS_Y, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL);
    if(window == NULL) {
        printf("Could not create the window: %s\n", SDL_GetError());
        exit(-1);
    }
}

// Add shader to the program
void addShader(GLuint program, const char* shaderCode, GLenum shaderType){
    GLuint theShader = glCreateShader(shaderType);

    const GLchar* theCode[1];
    theCode[0] = shaderCode;

    GLint codeLength[1];
    codeLength[0] = strlen(shaderCode);

    glShaderSource(theShader, 1, theCode, codeLength);
    glCompileShader(theShader);

    // Check to see if shader compiled
    GLint result = 0;

	GLchar eLog[1024] = { 0 };

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);

	if (!result) {
		glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
		printf("Error compiling the %d shader : '%s' \n", shaderType, eLog);
		return;
	}

    glAttachShader(program, theShader);

}

// Compile shaders
void compileShaders(){
    myProgram = glCreateProgram();
    if(!myProgram){
        printf("error creating shader program\n");
        return;
    }

    addShader(myProgram, vShader, GL_VERTEX_SHADER);
    addShader(myProgram, fShader, GL_FRAGMENT_SHADER);

    //Link shader
    GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glLinkProgram(myProgram);
	glGetProgramiv(myProgram, GL_LINK_STATUS, &result);

	if (!result) {
		glGetProgramInfoLog(myProgram, sizeof(eLog), NULL, eLog);
		printf("Error linking program : '%s' \n", eLog);
		return;
	}

    glValidateProgram(myProgram);
    glGetProgramiv(myProgram, GL_VALIDATE_STATUS, &result);

    if (!result) {
		glGetProgramInfoLog(myProgram, sizeof(eLog), NULL, eLog);
		printf("Error linking program : '%s' \n", eLog);
		return;
	}

}

// Create a stupid triangle
void createTriangle(){
    GLfloat vertices[] = {
        -0.2f, -0.2f, 0.0f,
        0.2f, -0.2f, 0.0f,
        0.0f, 0.2f, 0.0f
    };

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
            
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(0);

        // Unbind VBO
        glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Unbind VAO
	glBindVertexArray(0);
}

//Draw Triangle
void drawTriangle(){
    glUseProgram(myProgram);
        
        glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

    glUseProgram(0);

    SDL_GL_SwapWindow(window);
}

// Clear Window
void clearWindow(){

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

// Process Keyboard Input
void processKeyboard(){
    SDL_Event event;
        if (SDL_PollEvent(&event)){
            const Uint8 *state = SDL_GetKeyboardState(NULL);
            switch(event.type){
                case SDL_QUIT :
                    quitSDL();
                    break;

                case SDL_KEYDOWN : 
                    
                    if (state[SDL_SCANCODE_RIGHT]){
                        printf("right arrow pressed\n");
                    }
                    else if (state[SDL_SCANCODE_LEFT]){
                        printf("left arrow pressed\n");
                    }
                    else if (state[SDL_SCANCODE_DOWN]){
                        printf("down arrow pressed\n");
                    }
                    else if (state[SDL_SCANCODE_UP]){
                        printf("up arrow pressed\n");
                    }
                    break;

                default: 
                    break;    
            }
            
        }
}

// Poll for window close
int checkQuit(){
    SDL_PollEvent(&e);
        if (e.type == SDL_QUIT)
            return 1;
        //False
        return 0;
}



// Quit SDL
void quitSDL(){
    printf("Quitting SDL.\n");
    
    /* Shutdown all subsystems */

    SDL_DestroyWindow(window);

    SDL_Quit();
    
    printf("Quitting....\n");

    exit(0);
}