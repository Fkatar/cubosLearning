#include <cubos/engine/assets/plugin.hpp>
#include <cubos/engine/collisions/colliding_with.hpp>
#include <cubos/engine/defaults/plugin.hpp>
#include <cubos/engine/input/plugin.hpp>
#include <cubos/engine/render/lights/environment.hpp>
#include <cubos/engine/render/voxels/palette.hpp>
#include <cubos/engine/scene/plugin.hpp>
#include <cubos/engine/settings/plugin.hpp>
#include <cubos/engine/tools/plugin.hpp>
#include <cubos/engine/utils/free_camera/plugin.hpp>
#include <cubos/engine/voxels/plugin.hpp>

#include "obstacle.hpp"
#include "jetpack.hpp"
#include "player.hpp"
#include "spawner.hpp"

using namespace cubos::engine;

static const Asset<Scene> SceneAsset = AnyAsset("/assets/scenes/main.cubos");
static const Asset<VoxelPalette> PaletteAsset = AnyAsset("/assets/main.pal");
static const Asset<InputBindings> InputBindingsAsset = AnyAsset("/assets/input.bind");

int main(int argc, char** argv)
{
    Cubos cubos{argc, argv};

    cubos.plugin(defaultsPlugin);
    cubos.plugin(freeCameraPlugin);
    cubos.plugin(toolsPlugin);
    cubos.plugin(spawnerPlugin);
    cubos.plugin(obstaclePlugin);
    cubos.plugin(jetpackPlugin);
    cubos.plugin(playerPlugin);

    cubos.startupSystem("configure settings").before(settingsTag).call([](Settings& settings) {
        settings.setString("assets.app.osPath", APP_ASSETS_PATH);
        settings.setString("assets.builtin.osPath", BUILTIN_ASSETS_PATH);
    });

    cubos.startupSystem("set the palette, environment, input bindings and spawn the scene")
        .tagged(assetsTag)
        .call([](Commands commands, const Assets& assets, RenderPalette& palette, Input& input,
                 RenderEnvironment& environment) {
            palette.asset = PaletteAsset;
            environment.ambient = {0.1F, 0.1F, 0.1F};
            environment.skyGradient[0] = {0.2F, 0.4F, 0.8F};
            environment.skyGradient[1] = {0.6F, 0.6F, 0.8F};
            input.bind(*assets.read(InputBindingsAsset));
            commands.spawn(assets.read(SceneAsset)->blueprint);
        });

    cubos.system("restart the game on input")
        .call([](Commands cmds, const Assets& assets, const Input& input, Query<Entity> all, Query<Player&> entity) {
            
            for (auto [player] : entity) {
                if (player.health < 0) {
                    CUBOS_INFO("Player health is below 0, restarting", player.health);
                    for (auto [ent] : all)
                    {
                        cmds.destroy(ent);
                    }
                    Obstacle::resetSpeed();

                    cmds.spawn(assets.read(SceneAsset)->blueprint);
                }
            }
            
           (void)entity;
            if (input.justPressed("restart"))
            {
                for (auto [ent] : all)
                {
                    cmds.destroy(ent);
                }

                cmds.spawn(assets.read(SceneAsset)->blueprint);

                Obstacle::resetSpeed();
            }
        });

    cubos.system("detect player vs obstacle collisions")
        .call([](Query<Player&, const CollidingWith&, const Obstacle&> collisions) {
            for (auto [p, collidingWith, obstacle] : collisions)
            {
                CUBOS_INFO("Player collided with an obstacle!");
                // i want t odestroy the obstacle and decrease the player health, this is detroying thea player instead
                //redo this
                p.health--;
                CUBOS_INFO("Player health = {}", p.health);
                (void)p; // here to shut up 'unused variable warning', you can remove it
            }
            

        });

    cubos.run();

   

}
