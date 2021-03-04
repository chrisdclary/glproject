#ifndef INIT_H
#define INIT_H

#define WINDOWPOS_X         150
#define WINDOWPOS_Y         150
#define WINDOW_WIDTH        1280
#define WINDOW_HEIGHT       720

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

// Mouse Sensitivity
const float mouse_sensitivity = 0.07f;
    
class Object
{
public:
    int indexCount;                                     // number of indices, so the draw function knows where one object ends
    bool physics = false;                               // Whether physics act on this object
    bool collide = true;                                // Player can collide with this object
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);   // Position
    glm::vec3 size = glm::vec3(1.0f, 1.0f, 1.0f);       // Scale
    glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);   // Rotation - holds the degrees of rotations about each axis
    glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f);
    int state = 1;
    std::string type;
};  

// Axis-aligned bounding box
class AABB
{
public:
    glm::vec3 min = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 max = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 extents = glm::vec3(0.0f, 0.0f, 0.0f);

    AABB(glm::vec3 minimum, glm::vec3 maximum){
        min = minimum;
        max = maximum;
        pos.x = (min.x + (max.x - min.x) / 2);
        pos.y = (min.y + (max.y - min.y) / 2);
        pos.z = (min.z + (max.z - min.z) / 2);

        extents.x = (max.x - min.x) / 2;
        extents.y = (max.y - min.y) / 2;
        extents.z = (max.z - min.z) / 2;
    }
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
    glm::vec3 facing = glm::vec3(0.0f, 0.0f, -1.0f);
    int state = 0;
    // Player AABB always stays at the origin, this makes collision calculations a little easier
    AABB hitbox = AABB(glm::vec3(-1.0f, -4.0f, -1.0f), glm::vec3(1.0f, 4.0f, 1.0f));
};

// drawVerts holds vertex data BEFORE transformations i.e. vertex position in its own relative space
extern VertexContainer* drawVerts;
extern Player* player;

extern std::vector <Object>* AllObjects;

extern unsigned int deltaTime;


// Subsystem Prototypes
void initVideo();
void updateVideo();
void processKeyboard();
void playerMove(int);
void cameraMove(int, int);
void clearVideoBuffers();
void showFPS(int, unsigned int);
void initLevel();
void doPhysics();

// Engine prototypes
void initEngine();
void updateEngine();
void quitSDL();

#endif