#include "engine.h"

float yaw = -90.0f, pitch;
// **** Controls **** //

// Handle WASD 
void playerMove(int i, unsigned int delta)
{
    glm::vec3 globalUp = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 cameraRight = glm::normalize(glm::cross(globalUp, player->facing));
    // Modify movement amount based on delta time
    const float speed = 0.025f * delta;
    switch(i){
        case 0 : // W
            player->position += glm::normalize(glm::cross(cameraRight, globalUp)) * speed;
            break;
        case 1 : // A
            player->position -= glm::normalize(glm::cross(player->facing, globalUp)) * speed;
            break;
        case 2 : // S
            player->position -= glm::normalize(glm::cross(cameraRight, globalUp)) * speed;
            break;
        case 3 : // D
            player->position += glm::normalize(glm::cross(player->facing, globalUp)) * speed;
            break;
        case 4 : // Space
            if(player->state == 0){
                player->state = 1;
                player->velocity.y = 0.05f;
                AllObjects->at(1).state = 1;
                AllObjects->at(1).velocity.y = 0.025f;
            }
            
            break;
        case 5 : // Shift
            player->state = 0;
            break;
        default:
            break;
    }
}

// Handle mouse movement
void cameraMove(int x, int y)
{
    yaw += (float)x * mouse_sensitivity;
    pitch += (float)y * mouse_sensitivity;

    // Camera gets weird if the player looks too high/low
    if(pitch > 89.9f)
        pitch = 89.9f;
    else if(pitch < -89.9f)
        pitch = -89.9f;

    // Prevents overflow if player spins too much
    if(yaw > 360.0f)
        yaw -= 360.0f;
    else if(yaw < -360.0f)
        yaw += 360.0f;

    // Update camera vectors
    player->facing.x += cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    player->facing.y -= sin(glm::radians(pitch));
    player->facing.z += sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    player->facing = glm::normalize(player->facing);
}