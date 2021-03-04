#include "engine.h"
#include <SDL2/SDL.h>
#include <stdio.h>


// Event structure
SDL_Event e;

// Delta time & Frame time
unsigned int  frameTime = 0, lastTime = 0, deltaTime = 0;


// Initialize global objects
Player* player = new Player;
VertexContainer* drawVerts = new VertexContainer;
std::vector <Object>* AllObjects = new std::vector <Object>;

// Initialize framerate counter
int frames = 0; 

void initEngine()
{
    printf("Initializing SDL.\n");
    
    /* Initialize defaults, Video and Audio */

    if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) == -1) { 
        printf("Could not initialize SDL: %s.\n", SDL_GetError());
        exit(-1);
    }
    atexit(SDL_Quit);

    printf("SDL initialized.\n");

    // Relative mouse for camera controls
    SDL_SetRelativeMouseMode(SDL_TRUE);

    initLevel();

    // Initialize video subsystem
    initVideo();
}

// Process Keyboard Input
void processKeyboard(){
    const Uint8 *state = SDL_GetKeyboardState(NULL);
                    
    if (state[SDL_SCANCODE_D]){
        playerMove(3);
    }
    if (state[SDL_SCANCODE_A]){
        playerMove(1);
    }
    if (state[SDL_SCANCODE_S]){
        playerMove(2);
    }
    if (state[SDL_SCANCODE_W]){
        playerMove(0);
    }
    if (state[SDL_SCANCODE_SPACE]){
        playerMove(4);
    }
    if (state[SDL_SCANCODE_LSHIFT]){
        playerMove(5);
    }      
    if (state[SDL_SCANCODE_ESCAPE]){
        quitSDL();
    }
}

// Poll for events
void pollEvents()
{
    while (SDL_PollEvent(&e)){
        switch (e.type) {

            case SDL_QUIT :
                quitSDL();
                break;
            
            case SDL_MOUSEMOTION :
                cameraMove(e.motion.xrel, e.motion.yrel);
                break;

            default :
                break;
        }
    }
}

// Keep track of delta time and framerate
void updateTime()
{
    unsigned int currentTime = SDL_GetTicks();
    deltaTime = currentTime - lastTime;
    lastTime = currentTime;

    if (currentTime >= frameTime + 250){
        showFPS(frames, (currentTime - frameTime));
        frameTime = currentTime;
        frames = 0;
    }
    frames++;
}

// Quit SDL
void quitSDL()
{
    printf("Quitting SDL.\n");
    
    /* Shutdown all subsystems */
    clearVideoBuffers();
    SDL_Quit();
    
    printf("Quitting....\n");

    exit(0);
}

void updateEngine()
{
    updateTime();
    processKeyboard();
    doPhysics();
    pollEvents();
    updateVideo();
}