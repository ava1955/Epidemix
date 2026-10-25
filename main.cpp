#include <raylib.h>
#include <string>
#include <iostream>
#include <thread>
#include <cstdlib>
#include "config.h"
#include "oop/include/main_oop.h"
#include "dod/include/main_dod.h"
#include "dod/include/benchmark.h"

enum SimMode { MODE_OOP, MODE_DOD };

bool restart = false;

void readCommands() {
    std::string name, value;
    while (std::cin >> name >> value) {
        if (name == "entities") { config.EntityCount = std::atoi(value.c_str()); restart = true; }
        if (name == "speed") config.CircleSpeed = std::atof(value.c_str());
        if (name == "virus") { config.Virus = value == "on"; restart = true; }
    }
}

int main() {
    InitWindow(config.WINDOW_WIDTH, config.WINDOW_HEIGHT, "Puscarie");
    SetConfigFlags(FLAG_WINDOW_ALWAYS_RUN);
    SetTargetFPS(config.TargetFPS);

    OOPState oopState;
    DODState dodState;

    initDOD(&dodState);
    auto& oopFps = OOPFPSCounter::getInstance();
    auto& oopMem = OOPMemoryUsage::getInstance();

    DODFPSCounter* dodFps = FPSCounterGetInstance();
    DODMemoryUsage* dodMemory = MemoryUsageGetInstance();

    SimMode currentMode = MODE_OOP;

    std::thread(readCommands).detach();

    float shownFps = 0.0f;
    float shownMemory = 0.0f;
    float refreshTimer = 0.0f;

    while (!WindowShouldClose()) {
        if (restart) {
            restart = false;
            oopState.isInitialized = false;
            freeDODState(&dodState);
            initDOD(&dodState);
        }

        float dt = GetFrameTime();
        if (IsKeyPressed(KEY_SPACE)) {
            currentMode = (currentMode == MODE_OOP) ? MODE_DOD : MODE_OOP;
        }
        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (currentMode == MODE_OOP) {
            oop(oopState, dt);
        }
        else {
            dod(&dodState, dt);
        }
        std::string modeText = "Varianta curenta: ";
        modeText += (currentMode == MODE_OOP) ? "C++ (OOP)" : "C (DOD)";
        DrawText(modeText.c_str(), 20, 20, 24, BLACK);
        DrawText("Apasa SPACE pentru a schimba modul", 20, 50, 18, DARKGRAY);
        DrawText(TextFormat("FPS: %.0f", shownFps), 20, 80, 20, BLACK);
        DrawText(TextFormat("Memory: %.2f MB", shownMemory), 20, 105, 20, BLACK);

        EndDrawing();

        float fps;
        float memory;
        if (currentMode == MODE_OOP) {
            oopFps.update();
            fps = oopFps.getValue();
            memory = oopMem.getValue();
        }
        else {
            fpsUpdate(dodFps);
            fps = fpsGetValue(dodFps);
            memory = memGetValue(dodMemory);
        }

        refreshTimer += dt;
        if (refreshTimer >= 0.5f) {
            shownFps = fps;
            shownMemory = memory;
            refreshTimer = 0.0f;
        }
    }
    freeDODState(&dodState);
    CloseWindow();
    return 0;
}
