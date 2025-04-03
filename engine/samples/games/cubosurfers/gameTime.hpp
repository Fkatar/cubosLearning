#pragma once

#include <cubos/engine/prelude.hpp>

struct GameTime
{
    CUBOS_REFLECT;

    float timePassed = {0.0F};
};

void timePlugin(cubos::engine::Cubos& cubos);
