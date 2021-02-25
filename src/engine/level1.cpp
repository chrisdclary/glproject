#include "engine.h"
#include <stdio.h>

// This file contains all of the geometry for level 1

// Prototypes
void addVertsToBuffer();

// Load level data into drawVerts
void initLevel()
{

    printf("initializing level");
    unsigned int cur = 0; // Current location for IBO

    std::vector <float> groundData = {
        -100.0f, -2.0f, -100.0f,        1.0f, 0.0f, 0.0f,
        100.0f, -2.0f, -100.0f,         1.0f, 0.0f, 0.0f,
        100.0f, -2.0f, 100.0f,          0.0f, 0.0f, 1.0f,
        -100.0f, -2.0f, 100.0f,         0.0f, 0.0f, 1.0f
    };

    std::vector <unsigned int> groundIndex = {
        cur+0, cur+1, cur+2,
        cur+0, cur+2, cur+3
    };

    cur += (groundData.size() / 6); // there are 6 attributes per vertex, so this gives us the number of vertices

    std::vector <float> cubeData = {
        -1.0f, -1.0, 1.0f,              1.0f, 0.0f, 0.0f,
        1.0f, -1.0f, 1.0f,              0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 1.0f,               0.0f, 0.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,              1.0f, 1.0f, 0.0f,

        -1.0f, -1.0, -1.0f,              0.0f, 0.0f, 1.0f,
        1.0f, -1.0f, -1.0f,              1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, -1.0f,               1.0f, 0.0f, 0.0f,
        -1.0f, 1.0f, -1.0f,              0.0f, 1.0f, 0.0f
    };

    std::vector <unsigned int> cubeIndex = {
        cur+0, cur+1, cur+2,
        cur+2, cur+3, cur+0,
        cur+1, cur+5, cur+6, 
        cur+6, cur+2, cur+1,
        cur+5, cur+4, cur+7,
        cur+7, cur+6, cur+5,
        cur+4, cur+0, cur+3,
        cur+3, cur+7, cur+4,
        cur+3, cur+2, cur+6,
        cur+6, cur+7, cur+3,
        cur+4, cur+5, cur+1,
        cur+1, cur+0, cur+4,
    };

    // Add geometry to the list of vertices to be drawn
    drawVerts->vertices.insert(drawVerts->vertices.end(), groundData.begin(), groundData.end());
    drawVerts->indices.insert(drawVerts->indices.end(), groundIndex.begin(), groundIndex.end());
    drawVerts->vertices.insert(drawVerts->vertices.end(), cubeData.begin(), cubeData.end());
    drawVerts->indices.insert(drawVerts->indices.end(), cubeIndex.begin(), cubeIndex.end());

}

