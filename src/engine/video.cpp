#include "engine.h"
#include "glad/glad.h"
#include <SDL2/SDL_opengl.h>
#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// GL stuff
GLuint VAO, VBO, IBO, shaderProgram, uniformModel, uniformProjection, uniformView;


// **** Camera ****
float yaw = -90.0f, pitch;
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraTarget;
glm::vec3 cameraDir = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraRight;
glm::vec3 cameraUp;
// Global up vector is useful for player movement and some vector initialization
glm::vec3 globalUp = glm::vec3(0.0f, 1.0f, 0.0f);
// Settings for perspective camera
const float fov = 45.0f; // Camera field of view
const float near_plane = 0.1f; // Near clipping plane
const float far_plane = 100.0f; // Far clipping plane

// Miltiplier to get equilateral pyramid
const float oneRootTwo = 1/sqrt(2);

// Window
SDL_Window *window;


// **** Prototypes **** //

void addShader(GLuint, const char*, GLenum);
void compileShaders();
void createPyramid();

// **** Function Defs **** //

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

    // Set base background color
    glClearColor(0.1f, 0.64f, 1.0f, 1.0f);

    createPyramid();
}

// Add shader to the program
void addShader(GLuint program, const char* filename, GLenum shaderType)
{
    GLuint shader = glCreateShader(shaderType);

    std::stringstream ss;
    std::ifstream file;
    std::string codeString;
    const GLchar* shaderCode;

    try {
        file.open(filename, std::ios::in);
        if (!file.fail()) {
            ss << file.rdbuf();
            codeString = ss.str();
        }
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

// Handle WASD 
void playerMove(int i, unsigned int delta)
{
    // Modify movement amount based on delta time
    const float speed = 0.005f * delta;
    switch(i){
        case 0 : // W
            cameraPos += glm::normalize(glm::cross(cameraRight, globalUp)) * speed;
            break;
        case 1 : // A
            cameraPos -= glm::normalize(glm::cross(cameraDir, cameraUp)) * speed;
            break;
        case 2 : // S
            cameraPos -= glm::normalize(glm::cross(cameraRight, globalUp)) * speed;
            break;
        case 3 : // D
            cameraPos += glm::normalize(glm::cross(cameraDir, cameraUp)) * speed;
            break;
        case 4 : // Space
            cameraPos += globalUp * speed;
            break;
        case 5 : // Shift
            cameraPos -= globalUp * speed;
            break;
        default:
            break;
    }
}

// Handle mouse movement
void cameraMove(int x, int y)
{
    yaw += (float)x * mouse_sensitivity;
    pitch += (float)y * mouse_sensitivity;

    // Camera gets weird if the player looks too high/low
    if(pitch > 89.9f)
        pitch = 89.9f;
    else if(pitch < -89.9f)
        pitch = -89.9f;

    // Prevents overflow if player spins too much
    if(yaw > 360.0f)
        yaw -= 360.0f;
    else if(yaw < -360.0f)
        yaw += 360.0f;

    // Update camera vectors
    cameraDir.x += cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraDir.y -= sin(glm::radians(pitch));
    cameraDir.z += sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraDir = glm::normalize(cameraDir);
}


// Create a stupid pyramid
void createPyramid()
{
    // Draw order for IBO
    unsigned int indices[] = {
        3, 1, 0,
        3, 2, 0,
        0, 2, 1, 
        1, 2, 3
    };

    GLfloat vertices[] = {
        // Position                     // Color
        1.0f, 0.0f, oneRootTwo * -1,    1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, oneRootTwo * -1,   0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, oneRootTwo,         0.0f, 0.0f, 1.0f,
        0.0f, -1.0f, oneRootTwo,        1.0f, 1.0f, 0.0f
    };

    // Add these vertices to the array object
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Bind IBO and add indices
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // Bind VBO and add vertex data
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

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

// Update window title to show FPS
void showFPS(int frames, unsigned int time){
    int fps = (1000 / time) * frames;

    std::ostringstream outs;
    outs << std::fixed << "testgame.exe  |  FPS: " << fps;
    SDL_SetWindowTitle(window, outs.str().c_str());
}

// Draw function
void updateVideo()
{
    // Clear screen before draw
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shaderProgram);
    
    // Perspective camera
    glm::mat4 projection = glm::perspective(fov, (GLfloat)(WINDOW_WIDTH / WINDOW_HEIGHT), near_plane, far_plane);

    // **** Update camera vectors****
    // Direction vector is updated by mouse movement, 
    // so we only need to update the right and up vectors here
    cameraRight = glm::normalize(glm::cross(globalUp, cameraDir));
    cameraUp = glm::cross(cameraDir, cameraRight);


    // **** Model & Projection ****
    // Model manipulation matrix
    glm::mat4 model(1.0f);

    // Scale down
    model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));


    // Camera location, angle, etc.
    glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraDir, cameraUp);

    // Link model, view, and projection matrices to the vertex shader
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(view));
        
    // Draw elements in the buffer
    glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
            glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glUseProgram(0);

    // Window won't update without this
    SDL_GL_SwapWindow(window);
}

void clearVideoBuffers()
{
    glDeleteProgram(shaderProgram);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &IBO);
}