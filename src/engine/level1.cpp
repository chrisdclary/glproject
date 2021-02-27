#include "engine.h"
#include <stdio.h>

// This file contains all of the geometry for level 1

// Prototypes
void addVertsToBuffer();

unsigned int createCube(unsigned int);

// Load level data into drawVerts
void initLevel()
{
    // Player starting position
    player->position = glm::vec3(0.0f, 0.0f, 10.0f);

    printf("initializing level");
    unsigned int offset = 0;

    std::vector <float> groundData = {
        -100.0f, -2.0f, -100.0f,        1.0f, 0.0f, 0.0f,
        100.0f, -2.0f, -100.0f,         1.0f, 0.0f, 0.0f,
        100.0f, -2.0f, 100.0f,          0.0f, 0.0f, 1.0f,
        -100.0f, -2.0f, 100.0f,         0.0f, 0.0f, 1.0f
    };

    std::vector <unsigned int> groundIndex = {
        offset+0, offset+1, offset+2,
        offset+0, offset+2, offset+3
    };

    offset += (groundData.size() / 6); // there are 6 attributes per vertex, so this gives us the number of vertices

    Object level;
    level.indexCount  = groundIndex.size();
    AllObjects->push_back(level);

    // Add geometry to the list of vertices to be drawn
    drawVerts->vertices.insert(drawVerts->vertices.end(), groundData.begin(), groundData.end());
    drawVerts->indices.insert(drawVerts->indices.end(), groundIndex.begin(), groundIndex.end());

    offset = createCube(offset);
    offset = createCube(offset);
    offset = createCube(offset);
    offset = createCube(offset);
    offset = createCube(offset);
    offset = createCube(offset);
    offset = createCube(offset);
    offset = createCube(offset);
    offset = createCube(offset);


    AllObjects->at(0).position = glm::vec3(0.0f, -6.0f, 0.0f);
    AllObjects->at(1).physics = true;
    AllObjects->at(2).position = glm::vec3(3.0f, 3.0f, 3.0f);
    AllObjects->at(3).position = glm::vec3(3.0f, 3.0f, -3.0f);
    AllObjects->at(4).position = glm::vec3(-3.0f, 3.0f, -3.0f);
    AllObjects->at(5).position = glm::vec3(-3.0f, 3.0f, 3.0f);
    AllObjects->at(6).position = glm::vec3(3.0f, -3.0f, 3.0f);
    AllObjects->at(7).position = glm::vec3(3.0f, -3.0f, -3.0f);
    AllObjects->at(8).position = glm::vec3(-3.0f, -3.0f, -3.0f);
    AllObjects->at(9).position = glm::vec3(-3.0f, -3.0f, 3.0f);

}

