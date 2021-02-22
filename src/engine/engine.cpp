#include "engine.h"
#include <SDL2/SDL.h>
#include <stdio.h>

// Subsystem Prototypes
void initVideo();
void updateVideo();
void processKeyboard();
void playerMove(int, unsigned int);
void cameraMove(int, int);

// Player object
Player player;

// Event structure
SDL_Event e;

// Delta time 
unsigned int lastTime = 0, deltaTime = 0, currentTime;

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

    // Initialize video subsystem
    initVideo();

    // Initialize player values
    player = {{0.0f, 0.0f, 3.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, 0};
}

// Process Keyboard Input
void processKeyboard(){
    const Uint8 *state = SDL_GetKeyboardState(NULL);
                    
    if (state[SDL_SCANCODE_D]){
        playerMove(3, deltaTime);
    }
    if (state[SDL_SCANCODE_A]){
        playerMove(1, deltaTime);
    }
    if (state[SDL_SCANCODE_S]){
        playerMove(2, deltaTime);
    }
    if (state[SDL_SCANCODE_W]){
        playerMove(0, deltaTime);
    }
    if (state[SDL_SCANCODE_SPACE]){
        playerMove(4, deltaTime);
    }
    if (state[SDL_SCANCODE_LSHIFT]){
        playerMove(5, deltaTime);
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
}

// Quit SDL
void quitSDL()
{
    printf("Quitting SDL.\n");
    
    /* Shutdown all subsystems */

    SDL_Quit();
    
    printf("Quitting....\n");

    exit(0);
}

void updateEngine()
{
    updateVideo();
    updateTime();
    processKeyboard();
    pollEvents();
}