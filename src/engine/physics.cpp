#include "engine.h"

void doPhysics(unsigned int delta) 
{
    float gravity = pow(0.0098f, 2);

    // Do physics for any physics objects
    for( Object& o : *AllObjects){
        if(o.physics){
            o.position.y += o.velocity.y * (delta);
            o.velocity.y -= gravity * (delta);
        }
    }

    // Show velocity
    // std::cout << glm::to_string(player->velocity) << "\n";
    
    // Update player position based on velocity
    player->position.x += player->velocity.x * (delta);
    player->position.y += player->velocity.y * (delta);
    player->position.z += player->velocity.z * (delta);

    // Slow down player when they are not pressing movement keys
    player->velocity.x = player->velocity.x / 1.005;
    player->velocity.z = player->velocity.z / 1.005;

    // Apply gravity
    player->velocity.y -= gravity * (delta);

}

// **** Helper Functions for checkCollision **** //

bool testAxis(glm::vec3 testAxis, glm::vec3 v1, glm::vec3 v2, glm::vec3 v3)
{

    
    glm::vec3 xAxis = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 yAxis = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 zAxis = glm::vec3(0.0f, 0.0f, 1.0f);

    //Test collision for each axis
    // Project triangle vertices on first axis
    float p1 = glm::dot(v1, testAxis);
    float p2 = glm::dot(v2, testAxis);
    float p3 = glm::dot(v3, testAxis);

    float r = player->hitbox.extents.x * abs(glm::dot(xAxis, testAxis)) +
                player->hitbox.extents.y * abs(glm::dot(yAxis, testAxis)) +
                player->hitbox.extents.z * abs(glm::dot(zAxis, testAxis));
    
    if(std::max(-(std::max(std::max(p1, p2), p3)) , std::min(std::min(p1, p2), p3)) > r) {
        return false;
    }

    return true;
}

glm::vec3 getVertexData(unsigned int offset, glm::mat4 model)
{
    glm::vec4 temp;
    // Get vertex's position from drawVerts
    int cur = (drawVerts->indices[offset] * 6);
    temp.x = drawVerts->vertices[cur];
    temp.y = drawVerts->vertices[cur+1];
    temp.z = drawVerts->vertices[cur+2];
    temp.w = (1.0f); // Vertex represents a position in space, so w is 1

    // Apply model transform to get the vertex's absolute position in space
    temp = model * temp;

    // Subtract player position to get coordinates relative to player
    glm::vec3 temp2(temp);
    temp2 -= player->position;

    return temp2;
}


void checkCollision(unsigned int offset, glm::mat4 model)
{
    // Get vertex location relative to the player
    glm::vec3 v1 = getVertexData(offset, model);
    glm::vec3 v2 = getVertexData(offset+1, model);
    glm::vec3 v3 = getVertexData(offset+2, model);

    // **** For a collision to happen, there must be no separation across 13 axes **** //

    // 3 Cardinal directions / player hitbox normals
    glm::vec3 xAxis = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 yAxis = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 zAxis = glm::vec3(0.0f, 0.0f, 1.0f);

    // 9 Axes formed by crossing player hitbox's normals & triangle edges
    // Triangle edges
    glm::vec3 e1 = v2 - v1;
    glm::vec3 e2 = v3 - v2;
    glm::vec3 e3 = v1 - v3;

    glm::vec3 xAxis_e1 = glm::cross(xAxis, e1);
    glm::vec3 xAxis_e2 = glm::cross(xAxis, e2);
    glm::vec3 xAxis_e3 = glm::cross(xAxis, e3);

    glm::vec3 yAxis_e1 = glm::cross(yAxis, e1);
    glm::vec3 yAxis_e2 = glm::cross(yAxis, e2);
    glm::vec3 yAxis_e3 = glm::cross(yAxis, e3);

    glm::vec3 zAxis_e1 = glm::cross(zAxis, e1);
    glm::vec3 zAxis_e2 = glm::cross(zAxis, e2);
    glm::vec3 zAxis_e3 = glm::cross(zAxis, e3);

    // The triangle's normal itself
    glm::vec3 triNorm = glm::cross(e1, e2);
    triNorm = glm::normalize(triNorm);

    // Throw them all in a vector and check for separation against every axis
    std::vector <glm::vec3> axes;
    axes.push_back(xAxis_e1);
    axes.push_back(xAxis_e2);
    axes.push_back(xAxis_e3);
    axes.push_back(yAxis_e1);
    axes.push_back(yAxis_e2);
    axes.push_back(yAxis_e3);
    axes.push_back(zAxis_e1);
    axes.push_back(zAxis_e2);
    axes.push_back(zAxis_e3);
    axes.push_back(xAxis);
    axes.push_back(yAxis);
    axes.push_back(zAxis);
    axes.push_back(triNorm);

    for (glm::vec3 tempAxis : axes) {
        if(!testAxis(tempAxis, v1, v2, v3))     // If any axis shows separation, there is no collision
            return;
    }

    // **** If there is a collision, do stuff **** //

    // If the normal of the collided triangle is mostly facing vertically
    if((abs(triNorm.y) > abs(triNorm.x)) && (abs(triNorm.y) > abs(triNorm.z))) {
        player->position = player->position - player->velocity;
        player->velocity.y = 0.0f;
        if(triNorm.y > 0)
            player->state = 0;
    }

    if((abs(triNorm.x) > abs(triNorm.y)) && (abs(triNorm.x) > abs(triNorm.z))) {
        std::cout << "Position: " << glm::to_string(player->position) << "\n";
        std::cout << "Velocity: " << glm::to_string(player->velocity) << "\n";
        player->position.x -= player->velocity.x;
        player->velocity.x = 0.0f;
    }
    if((abs(triNorm.z) > abs(triNorm.y)) && (abs(triNorm.z) > abs(triNorm.x))) {
        player->position.z -= player->velocity.z;
        player->velocity.z = 0.0f;
    }

}


