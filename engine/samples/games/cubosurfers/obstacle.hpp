#pragma once

#include <glm/vec3.hpp>

#include <cubos/engine/prelude.hpp>

struct Obstacle
{
    CUBOS_REFLECT; // Ensure this macro is correctly defined in your project

    glm::vec3 velocity{0.0F, 0.0F, -1.0F};
    float killZ{0.0F};
    static float speedIncrease;

      // Static method to increase the speed
      static void increaseSpeed()
      {
          speedIncrease += 0.1F; // Increase speed by 0.1 each time it's called
      }

      static void resetSpeed()
      {
          speedIncrease = 1.0F; // Reset speed to 1.0
      }
    
};


void obstaclePlugin(cubos::engine::Cubos& cubos);
