#include <iostream>
#include <memory>
#include <vector>
#include <raylib.h>
#include "entity.h"
#include "benchmark.h"

constexpr int WINDOW_WIDTH = 1800;
constexpr int WINDOW_HEIGHT = 1200;

int main() {
    std::vector<Entity*> entities;
    for (auto i = 0; i < 20000; i++ ) {
        entities.push_back(new Circle(rand() % WINDOW_WIDTH,rand() % WINDOW_HEIGHT,true,rand() % 360 ));
    }
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Puscarie");
    SetTargetFPS(120);
    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        BeginDrawing();
        ClearBackground(RAYWHITE);
        for (auto i = 0; i < entities.size(); i++) {
            entities[i]->movement(dt);
            DrawCircle(entities[i]->position.first,entities[i]->position.second,3,RED);
        }
        EndDrawing();
    }
    CloseWindow();
}
