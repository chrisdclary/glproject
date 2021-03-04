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
    player->position = glm::vec3(0.0f, 4.0f, 10.0f);

    printf("initializing level");
    unsigned int offset = 0;

    std::vector <float> groundData = {
        -100.0f, 0.0f, -100.0f,        1.0f, 0.0f, 0.0f,
        100.0f, 0.0f, -100.0f,         1.0f, 0.0f, 0.0f,
        100.0f, 0.0f, 100.0f,          0.0f, 0.0f, 1.0f,
        -100.0f, 0.0f, 100.0f,         0.0f, 0.0f, 1.0f
    };

    std::vector <unsigned int> groundIndex = {
        offset+0, offset+2, offset+1,
        offset+0, offset+3, offset+2,
    };

    offset += (groundData.size() / 6); // there are 6 attributes per vertex, so this gives us the number of vertices

    Object level;
    level.indexCount  = groundIndex.size();
    AllObjects->push_back(level);

    // Add geometry to the list of vertices to be drawn
    drawVerts->vertices.insert(drawVerts->vertices.end(), groundData.begin(), groundData.end());
    drawVerts->indices.insert(drawVerts->indices.end(), groundIndex.begin(), groundIndex.end());

    int i;
    for (i = 1; i <= 20; i++){
        offset = createCube(offset);
        AllObjects->at(i).size = glm::vec3(5.0f, 1.0f, 5.0f);
        AllObjects->at(i).rotation.y = i * 72.0f;
    }

    AllObjects->at(1).position = glm::vec3(10.0f, 8.0f, 10.0f);
    AllObjects->at(2).position = glm::vec3(15.0f, 16.0f, -10.0f);
    AllObjects->at(3).position = glm::vec3(0.0f, 24.0f, -20.0f);
    AllObjects->at(4).position = glm::vec3(-15.0f, 32.0f, -10.0f);
    AllObjects->at(5).position = glm::vec3(-10.0f, 40.0f, 10.0f);
    AllObjects->at(6).position = glm::vec3(10.0f, 48.0f, 10.0f);
    AllObjects->at(7).position = glm::vec3(15.0f, 56.0f, -10.0f);
    AllObjects->at(8).position = glm::vec3(0.0f, 64.0f, -20.0f);
    AllObjects->at(9).position = glm::vec3(-15.0f, 72.0f, -10.0f);
    AllObjects->at(10).position = glm::vec3(-10.0f, 80.0f, 10.0f);

    AllObjects->at(11).position = glm::vec3(10.0f, 88.0f, 10.0f);
    AllObjects->at(12).position = glm::vec3(15.0f, 96.0f, -10.0f);
    AllObjects->at(13).position = glm::vec3(0.0f, 104.0f, -20.0f);
    AllObjects->at(14).position = glm::vec3(-15.0f, 112.0f, -10.0f);
    AllObjects->at(15).position = glm::vec3(-10.0f, 120.0f, 10.0f);
    AllObjects->at(16).position = glm::vec3(10.0f, 128.0f, 10.0f);
    AllObjects->at(17).position = glm::vec3(15.0f, 136.0f, -10.0f);
    AllObjects->at(18).position = glm::vec3(0.0f, 144.0f, -20.0f);
    AllObjects->at(19).position = glm::vec3(-15.0f, 152.0f, -10.0f);
    AllObjects->at(20).position = glm::vec3(-10.0f, 160.0f, 10.0f);

    offset = createCube(offset); // Upright cube as a wall
    AllObjects->at(i).size = glm::vec3(1.0f, 100.0f, 50.0f);
    AllObjects->at(i).position = glm::vec3(50.0f, 100.0f, 0.0f);

    offset = createCube(offset); // rotated cube as a wall
    AllObjects->at(i+1).size = glm::vec3(100.0f, 1.0f, 50.0f);
    AllObjects->at(i+1).position = glm::vec3(-50.0f, 100.0f, 0.0f);
    AllObjects->at(i+1).rotation.z = 40.0f;
    

}

