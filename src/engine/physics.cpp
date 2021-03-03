#include "engine.h"

void doPhysics(unsigned int delta) 
{
    float gravity = pow(0.0098f, 2);

    if( player->velocity.y > 0.5f) // Cap velocity at .5 units
        player->velocity.y = 0.5f;

    for( Object& o : *AllObjects){
        if(o.physics){
            o.position.y += o.velocity.y * (delta);
            o.velocity.y -= gravity * (delta);

            // if(o.position.y <= -4.0f) { // o has landed
            //     o.position.y = -4.0f;
            //     o.velocity.y = 0.0f;
            //     o.state = 0;
            // }
        }
    }
    

    player->position.y += player->velocity.y * (delta);
    player->velocity.y -= gravity * (delta);

    // if(player->position.y <= -4.0f) { // Player has landed
    //     player->velocity.y = 0.0f;
    //     player->state = 0;
    // }

}


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


void checkCollision(unsigned int offset, glm::mat4 model)
{
    // 3 points of the triangle to be checked
    glm::vec4 v1_2;
    glm::vec4 v2_2;
    glm::vec4 v3_2;

    // Retrieve vertex data from drawVerts
    int cur = (drawVerts->indices[offset] * 6);
    float temp = drawVerts->vertices[cur];
    v1_2.x = temp;
    temp = drawVerts->vertices[cur+1];
    v1_2.y = temp;
    temp = drawVerts->vertices[cur+2];
    v1_2.z = temp;

    cur = (drawVerts->indices[offset+1] * 6);
    temp = drawVerts->vertices[cur];
    v2_2.x = temp;
    temp = drawVerts->vertices[cur+1];
    v2_2.y = temp;
    temp = drawVerts->vertices[cur+2];
    v2_2.z = temp;

    cur = (drawVerts->indices[offset+2] * 6);
    temp = drawVerts->vertices[cur];
    v3_2.x = temp;
    temp = drawVerts->vertices[cur+1];
    v3_2.y = temp;
    temp = drawVerts->vertices[cur+2];
    v3_2.z = temp;

    // Gross hacky stuff to apply matrix transformations to a vec3

    v1_2.w = 1.0f;
    v2_2.w = 1.0f;
    v3_2.w = 1.0f;

    glm::vec4 v1_3 = model * v1_2;
    glm::vec4 v2_3 = model * v2_2;
    glm::vec4 v3_3 = model * v3_2;

    glm::vec3 v1(v1_3);
    glm::vec3 v2(v2_3);
    glm::vec3 v3(v3_3);

    v1 -= player->position;
    v2 -= player->position;
    v3 -= player->position;

    // Triangle edges
    glm::vec3 e1 = v2 - v1;
    glm::vec3 e2 = v3 - v2;
    glm::vec3 e3 = v1 - v3;

    //printf("Collision data:\nTriangle - v1: %f, %f, %f  v2: %f, %f, %f  v3: %f, %f, %f", v1.x, v1.y, v1.z, v2.x, v2.y, v2.z, v3.x, v3.y, v3.z);

    glm::vec3 xAxis = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 yAxis = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 zAxis = glm::vec3(0.0f, 0.0f, 1.0f);

    // Compute the 9 axis (player AABB normals to the triangle edges)
    glm::vec3 xAxis_e1 = glm::cross(xAxis, e1);
    glm::vec3 xAxis_e2 = glm::cross(xAxis, e2);
    glm::vec3 xAxis_e3 = glm::cross(xAxis, e3);

    glm::vec3 yAxis_e1 = glm::cross(yAxis, e1);
    glm::vec3 yAxis_e2 = glm::cross(yAxis, e2);
    glm::vec3 yAxis_e3 = glm::cross(yAxis, e3);

    glm::vec3 zAxis_e1 = glm::cross(zAxis, e1);
    glm::vec3 zAxis_e2 = glm::cross(zAxis, e2);
    glm::vec3 zAxis_e3 = glm::cross(zAxis, e3);

    glm::vec3 triNorm = glm::cross(e1, e2);

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
        if(!testAxis(tempAxis, v1, v2, v3)) 
            return;
    }

    //printf("Relative position: %f, %f, %f\n", v1.x, v1.y, v1.z);
    player->velocity.y = 0.0f;
    player->state = 0;

}
