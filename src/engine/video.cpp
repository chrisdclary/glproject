#include "engine.h"
#include "glad/glad.h"
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL.h>
#include <string>
#include <sstream>
#include <fstream>
#include <stdio.h>
#include <glm/gtc/type_ptr.hpp>

// GL stuff
GLuint VAO, VBO, IBO, shaderProgram, uniformModel, uniformProjection, uniformView;

// **** Camera **** //
glm::vec3 cameraUp;
// Settings for perspective camera
const float fov = 60.0f; // Camera field of view
const float near_plane = 0.1f; // Near clipping plane
const float far_plane = 400.0f; // Far clipping plane


// Window
SDL_Window *window;

// **** Prototypes **** //

void addShader(GLuint, const char*, GLenum);
void compileShaders();
void addVertsToBuffer();
void checkCollision(unsigned int, glm::mat4);


/****************************
// **** Function Defs **** //
****************************/

// Initialize Video Subsystem
void initVideo()
{
    window = SDL_CreateWindow("testgame.exe", WINDOWPOS_X, WINDOWPOS_Y, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL);
    if(window == NULL) {
        printf("Could not create the window: %s\n", SDL_GetError());
        exit(-1);
    }
    
    // Setup GL Context
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    if(glContext == NULL){
        printf("Opengl context failed to initialize: %s\n", SDL_GetError());
        exit(-1);
    }

    // Get OpenGL function pointers
    gladLoadGLLoader(SDL_GL_GetProcAddress);

    // Compile vertex and fragment shaders
    compileShaders();

    // Set viewport to window size
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    // Set base background color
    glClearColor(0.1f, 0.64f, 1.0f, 1.0f);

    addVertsToBuffer();
}

// **** Shaders **** //

// Add shader to the program
void addShader(GLuint program, const char* filename, GLenum shaderType)
{
    GLuint shader = glCreateShader(shaderType);

    // Grab our source code from the shader file
    std::stringstream ss;
    std::ifstream file;
    std::string codeString; // If the code isn't saved like this, it won't work
    const GLchar* shaderCode;
    try {
        file.open(filename, std::ios::in);
        if (!file.fail()) {
            ss << file.rdbuf();
            codeString = ss.str(); 
        }
        file.close();
    } catch (std::exception ex) {
        printf("Failed to find shader code.");
    }

    shaderCode = codeString.c_str();

    glShaderSource(shader, 1, &shaderCode, NULL);
    glCompileShader(shader);

    // Check to see if shader compiled
    GLint result = 0;
	GLchar eLog[1024] = { 0 };
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("Error compiling the %d shader : '%s' \n", shaderType, eLog);
		return;
	}

    glAttachShader(program, shader);
}

// Compile shaders into shader program
void compileShaders()
{
    shaderProgram = glCreateProgram();
    if(!shaderProgram){
        printf("error creating shader program\n");
        return;
    }

    // Add shaders and link to shader program
    // ---------------------------------------------------------- This needs to be modified to use the linked object files
    addShader(shaderProgram, "src/engine/shaders/shader.vert", GL_VERTEX_SHADER);
    addShader(shaderProgram, "src/engine/shaders/shader.frag", GL_FRAGMENT_SHADER);

    GLint result = 0;
	GLchar eLog[1024] = { 0 };
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(shaderProgram, sizeof(eLog), NULL, eLog);
		printf("Error linking program : '%s' \n", eLog);
		return;
	}

    glValidateProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_VALIDATE_STATUS, &result);
    if (!result) {
		glGetProgramInfoLog(shaderProgram, sizeof(eLog), NULL, eLog);
		printf("Error linking program : '%s' \n", eLog);
		return;
	}

    // Get locations for shader variables
    uniformModel = glGetUniformLocation(shaderProgram, "u_ModelMatrix");
    uniformProjection = glGetUniformLocation(shaderProgram, "u_ProjectionMatrix");
    uniformView = glGetUniformLocation(shaderProgram, "u_ViewMatrix");

    glEnable(GL_DEPTH_TEST);

}

// Load vertex data from drawVerts into VBO
void addVertsToBuffer()
{

    // Add these vertices to the array object
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Bind IBO 
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    
    // Bind VBO 
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    for (float f : drawVerts->vertices)
        printf("%f, ", f);
    
    printf("\n");

    for (unsigned int i : drawVerts->indices)
        printf("%d, ", i);
    
    printf("\n");

    // Add vertex data from AllObjects to the buffers   
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, drawVerts->indices.size() * sizeof(unsigned int), drawVerts->indices.data(), GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, drawVerts->vertices.size() * sizeof(GLfloat), drawVerts->vertices.data(), GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, 0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (GLvoid*)(sizeof(GLfloat) * 3));
    glEnableVertexAttribArray(1);

    // Unbind VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Unbind VAO
	glBindVertexArray(0);

    // Unbind IBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


// Draw function
void updateVideo()
{
    // Clear screen before draw
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shaderProgram);

    // **** Update camera vectors****
    // Direction vector is updated by mouse movement, 
    // so we only need to update the right and up vectors here
    cameraUp = glm::cross(player->facing, glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), player->facing)));


    // **** Model & Projection ****
    // Model manipulation matrix
    glm::mat4 model(1.0f);

    // Perspective camera
    glm::mat4 projection = glm::perspective(glm::radians(fov), (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, near_plane, far_plane);

    // Camera location, angle, etc.
    glm::mat4 view = glm::lookAt(player->position, player->position + player->facing, cameraUp);

    // Link model, view, and projection matrices to the vertex shader
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(view));
        
    // Bind VAO and IBO
    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    // Draw all objects
    unsigned int offset = 0;
    for(Object o : *AllObjects){
        model = glm::translate(glm::mat4(1.0f), o.position);
        model = glm::rotate(model, glm::radians(o.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(o.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(o.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, o.size);
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        for(int i = 0; i < o.indexCount; i += 3){
            if(o.collide) // Check collision if object is collidable
                checkCollision(offset + i, model);
        }
        glDrawElements(GL_TRIANGLES, o.indexCount, GL_UNSIGNED_INT, (void*)(offset * sizeof(GLuint)));
        offset += o.indexCount;
    }

    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glUseProgram(0);

    // Window won't update without this
    SDL_GL_SwapWindow(window);
}


// **** Utility functions **** //

// Update window title to show FPS
void showFPS(int frames, unsigned int time){
    int fps = (1000 / time) * frames;

    std::ostringstream outs;
    outs << std::fixed << "testgame.exe  |  FPS: " << fps;
    SDL_SetWindowTitle(window, outs.str().c_str());
}

// Delete everything on program exit
void clearVideoBuffers()
{
    glDeleteProgram(shaderProgram);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &IBO);
}