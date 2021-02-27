#include "engine.h"

unsigned int createCube(unsigned int offset) {
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
        offset+0, offset+1, offset+2,
        offset+2, offset+3, offset+0,
        offset+1, offset+5, offset+6, 
        offset+6, offset+2, offset+1,
        offset+5, offset+4, offset+7,
        offset+7, offset+6, offset+5,
        offset+4, offset+0, offset+3,
        offset+3, offset+7, offset+4,
        offset+3, offset+2, offset+6,
        offset+6, offset+7, offset+3,
        offset+4, offset+5, offset+1,
        offset+1, offset+0, offset+4,
    };

    Object cube;
    cube.indexCount  = cubeIndex.size();
    cube.type = "cube";
    AllObjects->push_back(cube);

    drawVerts->vertices.insert(drawVerts->vertices.end(), cubeData.begin(), cubeData.end());
    drawVerts->indices.insert(drawVerts->indices.end(), cubeIndex.begin(), cubeIndex.end());

    return (offset + (cubeData.size() / 6));
}