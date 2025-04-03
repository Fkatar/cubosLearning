#pragma once

#include <glm/vec3.hpp>

#include <cubos/engine/prelude.hpp>

struct Obstacle
{
    CUBOS_REFLECT; // Ensure this macro is correctly defined in your project

    glm::vec3 velocity{0.0F, 0.0F, -1.0F};
    float killZ{0.0F};
    float speedIncrease{1.0F};

    
};


void obstaclePlugin(cubos::engine::Cubos& cubos);
