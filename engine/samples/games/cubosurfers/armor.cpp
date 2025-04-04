#include "armor.hpp"

#include <cubos/core/ecs/reflection.hpp>
#include <cubos/core/reflection/external/glm.hpp>
#include <cubos/core/reflection/external/primitives.hpp>
#include <cubos/core/reflection/external/string.hpp>

#include <cubos/engine/assets/plugin.hpp>
#include <cubos/engine/transform/plugin.hpp>

using namespace cubos::engine;

CUBOS_REFLECT_IMPL(Armor){
    return cubos::core::ecs::TypeBuilder<Armor>("Armor")
        .withField("velocity", &Armor::velocity)
        .withField("killZ", &Armor::killZ)
        .build();
}

void armorPlugin(Cubos& cubos){
    cubos.depends(assetsPlugin);
    cubos.depends(transformPlugin);

    cubos.component<Armor>();

    cubos.system("move armor")
        .call([](Commands cmds, const DeltaTime& dt, Query<Entity, const Armor&, Position&> obstacles) {
            for (auto [ent, armor, position] : obstacles)
            {
                position.vec += armor.velocity * dt.value();
                position.vec.y = glm::abs(glm::sin(position.vec.z * 0.15F)) * 1.5F;

                if (position.vec.z < armor.killZ)
                {
                    cmds.destroy(ent);
                }
            }
        });
}