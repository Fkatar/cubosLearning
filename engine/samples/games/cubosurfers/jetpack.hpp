#pragma once

#include <glm/vec3.hpp>

#include <cubos/engine/prelude.hpp>

struct Jetpack
{
    CUBOS_REFLECT;

    glm::vec3 velocity{0.0F, 0.0F, 0.0F};
    float killZ{0.0F};
    /* data */
};

void jetpackPlugin(cubos::engine::Cubos& cubos); // Ensure that cubos::engine::Cubos is defined in the included headers
