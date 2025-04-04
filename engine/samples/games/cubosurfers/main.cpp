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

#include <cubos/engine/transform/plugin.hpp>

#include "obstacle.hpp"
#include "jetpack.hpp"
#include "player.hpp"
#include "spawner.hpp"
#include "gameTime.hpp"
#include "armor.hpp"

#include <unistd.h>

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
    cubos.plugin(obstaclePlugin);
    cubos.plugin(jetpackPlugin);
    cubos.plugin(spawnerPlugin);
    cubos.plugin(playerPlugin);
    cubos.plugin(timePlugin);
    cubos.plugin(armorPlugin);

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
        .call([](Commands cmds, const Assets& assets, const Input& input, Query<Entity> all, Query<Player&> entity, GameTime& time) {
            
            for (auto [player] : entity) {
                if (player.health <= 0) {
                    CUBOS_INFO("Player health is below 0, restarting", player.health);
                    for (auto [ent] : all)
                    {
                        cmds.destroy(ent);
                    }
                    time.timePassed = 0.0F;
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
                time.timePassed = 0.0F;
                cmds.spawn(assets.read(SceneAsset)->blueprint);
            }
        });

    cubos.system("detect player vs obstacle collisions")
        .call([](Query<Player&, const CollidingWith&, Obstacle&> collisions) {
            for (auto [p, collidingWith, obstacle] : collisions)
            {
                obstacle.killZ = 1000;
                if (p.armored) 
                    p.armored = false;
                else 
                    p.health--;
               
            }

        });


    cubos.system("jetpack powerup")
        .call([] (Query<Player&, const CollidingWith&, const Jetpack&> colision, Query<Player&, Position&> players) {
            for (auto [player, collidingWith, jetpack] : colision) {

                for (auto [p, position] : players) {
                    position.vec.y = position.vec.y + 5;
                    p.jetPackTimer = 5.0F;
                    CUBOS_INFO("Time started: ",  p.jetPackTimer );
                    p.jetPackEffect = true;
            }
            (void)collidingWith;

        }
             
            
    });

    cubos.system("increase gameSpeed") 
        .call([] (Query<Obstacle&> obstacles, GameTime& time) 
        {
            for (auto [obstacle]:  obstacles){
                float velocityIncrease = glm::clamp( time.timePassed / 20, 0.0f,2.0f);
                obstacle.speedIncrease =  1 + velocityIncrease;
            }
        });


    cubos.system("update jetpack")
        .call([] (Query<Player&, Position&> players, DeltaTime& time)
        {
            for (auto  [player, position]: players){
                if(player.jetPackEffect) {

                    player.jetPackTimer -= time.value();
                    CUBOS_INFO("Decreasing jetpack timer: ",  player.jetPackTimer );
                }
                if(player.jetPackEffect && player.jetPackTimer < 0) {
                    CUBOS_INFO("Reseting jetpack");
                    player.jetPackTimer = 0;
                    player.jetPackEffect = false;
                    position.vec.y -= 5;
                }
            }
        });

    cubos.system("armor powerup")
        .call([] (Query<Player& , const CollidingWith&, const Armor&> colison) {
            for (auto [player, collidingWith, armor]: colison) {
                player.armored = true;
                (void)collidingWith;
                (void)armor;
            }
    });

    cubos.run();


   

}
