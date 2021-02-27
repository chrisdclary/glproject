#include "engine.h"

void doPhysics(unsigned int delta) 
{
    if (player->state == 1) { // In the air
        player->position.y += player->velocity.y * (delta);
        glm::vec3 acceleration = glm::vec3(0.0f, 0.0098f, 0.0f);
        player->velocity.y -= pow(acceleration.y, 2) * (delta);

        if(player->position.y <= -3.0f) { // Player has landed
            player->position.y = -3.0f;
            player->velocity.y = 0.0f;
            player->state = 0;
        }
    }
}