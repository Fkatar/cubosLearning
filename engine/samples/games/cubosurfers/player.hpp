#pragma once

#include <cubos/engine/prelude.hpp>

struct Player
{
    CUBOS_REFLECT;

    float jetPackTimer{0.0F};
    bool jetPackEffect{false}; // time remaining for the player in the air
    bool armored{false};       // is player armored ? 
    float speed{1.0F};         // Speed of the player
    float laneWidth{1.0F};     // Width of the lane
    int lane{0};               // Current lane
    int targetLane{0};         // Target lane
    int health{1};             // Starting health of the player
};

void playerPlugin(cubos::engine::Cubos& cubos);

void playerJetpackPlugin(cubos::engine::Cubos& cubos);
