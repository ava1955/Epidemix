#include "config.h"

Config config = {
    .WINDOW_WIDTH = 2880,
    .WINDOW_HEIGHT = 1800,
    .CircleRadius = 2,
    .CircleSpeed = 100.0f,
    .EntityCount = 50000,
    .TargetFPS = 1000,
    .ObstacleSize = 10,
    .ObstacleCount = 5,
    .Obstacles = {
        {300, 250},
        {1500, 250},
        {910, 490},
        {300, 750},
        {1500, 750},
    },
    .Virus = 1,
    .VirusLifetime = 5.0f,
    .InfectionThreshold = 100,
    .InfectionChance = 30,
};
