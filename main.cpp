#include <raylib.h>
#include <string>
#include <iostream>
#include "oop/include/main_oop.h"
#include "dod/include/main_dod.h"
#include "dod/include/benchmark.h"

enum SimMode { MODE_OOP, MODE_DOD };

int main() {
    InitWindow(1800, 1200, "Puscarie");
    SetConfigFlags(FLAG_WINDOW_ALWAYS_RUN);
    SetTargetFPS(10000);

    OOPState oopState;
    DODState dodState;

    initDOD(&dodState);
    auto& oopFps = OOPFPSCounter::getInstance();
    auto& oopMem = OOPMemoryUsage::getInstance();

    DODFPSCounter* dodFps = FPSCounterGetInstance();
    DODMemoryUsage* dodMemory = MemoryUsageGetInstance();

    SimMode currentMode = MODE_OOP;

    while (!WindowShouldClose()) {
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

        EndDrawing();
        if (currentMode == MODE_OOP) {
            oopFps.update();
            std::cout << "FPS: " << oopFps.getValue() << " | Memory: " << oopMem.getValue() << " MB | OOP\n";
        }
        else {
            fpsUpdate(dodFps);
            std::cout << "FPS: " << fpsGetValue(dodFps) << " | Memory: " << memGetValue(dodMemory) << " MB | DOD\n";
        }
    }
    freeDODState(&dodState);
    CloseWindow();
    return 0;
}
