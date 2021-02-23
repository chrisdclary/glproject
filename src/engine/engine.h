#ifndef INIT_H
#define INIT_H

#define WINDOWPOS_X         300
#define WINDOWPOS_Y         300
#define WINDOW_WIDTH        800
#define WINDOW_HEIGHT       600

// Mouse Sensitivity
const float mouse_sensitivity = 0.07f;

struct Player 
{
    float position[3];
    float velocity[3];
    float facing[3]; // May change this to different data type
    int state;

};

extern struct Player player;

void initEngine();
void updateEngine();
void quitSDL();

#endif