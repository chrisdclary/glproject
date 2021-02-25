#ifndef INIT_H
#define INIT_H

#define WINDOWPOS_X         150
#define WINDOWPOS_Y         150
#define WINDOW_WIDTH        1280
#define WINDOW_HEIGHT       720

#include <glm/glm.hpp>
#include <vector>

// Mouse Sensitivity
const float mouse_sensitivity = 0.07f;


    
// Contains all vertex data for the current level
class VertexContainer
{
public:
    std::vector <float> vertices; // Contains data in the format:
                                    // pos.x, pos.y, pos.z, R, G, B
    std::vector <unsigned int> indices;
};

class Player
{
public:
    glm::vec3 position;
    float velocity[3];
    float facing[3];            // May change this to different data type
    int state = 0;
};

extern VertexContainer* drawVerts;
extern Player* player;


// Subsystem Prototypes
void initVideo();
void updateVideo();
void processKeyboard();
void playerMove(int, unsigned int);
void cameraMove(int, int);
void clearVideoBuffers();
void showFPS(int, unsigned int);
void initLevel();

// Engine prototypes
void initEngine();
void updateEngine();
void quitSDL();

#endif