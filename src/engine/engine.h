#ifndef INIT_H
#define INIT_H

#define WINDOWPOS_X         150
#define WINDOWPOS_Y         150
#define WINDOW_WIDTH        1280
#define WINDOW_HEIGHT       720

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <vector>
#include <cmath>

// Mouse Sensitivity
const float mouse_sensitivity = 0.07f;
    
class Object
{
public:
    int indexCount;                                     // number of indices, so the draw function knows where one object ends
    bool physics = false;                               // Whether physics act on this object
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);   // Position
    glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f);
    int state = 0;
    std::string type;
};  

// Contains all vertex data for the current level
class VertexContainer
{
public:
    std::vector <float> vertices;                       // Contains data in the format: pos.x, pos.y, pos.z, R, G, B
    std::vector <unsigned int> indices;
};

class Player
{
public:
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f);
    float facing[3];                                    // May change this to different data type
    int state = 1;
};

// To be used throughout the engine
extern VertexContainer* drawVerts;
extern Player* player;

extern std::vector <Object>* AllObjects;


// Subsystem Prototypes
void initVideo();
void updateVideo();
void processKeyboard();
void playerMove(int, unsigned int);
void cameraMove(int, int);
void clearVideoBuffers();
void showFPS(int, unsigned int);
void initLevel();
void doPhysics(unsigned int);

// Engine prototypes
void initEngine();
void updateEngine();
void quitSDL();

#endif