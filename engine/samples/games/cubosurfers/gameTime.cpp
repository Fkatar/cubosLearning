#include "gameTime.hpp"

#include <cubos/core/ecs/reflection.hpp>
#include <cubos/core/reflection/external/primitives.hpp>

#include <cubos/engine/input/plugin.hpp>
#include <cubos/engine/transform/plugin.hpp>

using namespace cubos::engine;

CUBOS_REFLECT_IMPL(GameTime)
{
    return cubos::core::ecs::TypeBuilder<GameTime>("GameTime")
    .withField("timePassed", &GameTime::timePassed)
    .build();
}

void timePlugin(cubos::engine::Cubos& cubos)
{
    cubos.resource<GameTime>();

    cubos.system("increment time passed").call([](GameTime& time, const DeltaTime& dt)
    {
        time.timePassed += dt.value();
        CUBOS_INFO("Updating time passed: {}", time.timePassed); 
    });
}


