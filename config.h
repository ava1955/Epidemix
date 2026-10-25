#pragma once

#define MAX_OBSTACLES 32

typedef struct Config {
    int WINDOW_WIDTH;
    int WINDOW_HEIGHT;
    int CircleRadius;
    float CircleSpeed;
    int EntityCount;
    int TargetFPS;
    int ObstacleSize;
    int ObstacleCount;
    float Obstacles[MAX_OBSTACLES][2];
    int Virus;
    float VirusLifetime;
} Config;

extern Config config;
