#include "oop/include/main_oop.h"

constexpr int WINDOW_WIDTH = 1800;
constexpr int WINDOW_HEIGHT = 1200;

void InitOOP(OOPState& state) {
    state.entities.clear();
    for (auto i = 0; i < 100000; i++) {
        state.entities.push_back(std::make_unique<Circle>(rand() % WINDOW_WIDTH, rand() % WINDOW_HEIGHT, true, rand() % 360));
    }
    state.deleteTimer = 0.0f;
    state.isInitialized = true;
}

void oop(OOPState& state, float dt) {
    if (!state.isInitialized) {
        InitOOP(state);
    }

    state.deleteTimer += dt;
    if (state.deleteTimer >= 2.0f) {
        for (int k = 0; k < 100 && !state.entities.empty(); k++) {
            state.entities.pop_back();
        }
        state.deleteTimer = 0.0f;
    }

    for (size_t i = 0; i < state.entities.size(); i++) {
        state.entities[i]->movement(dt);
        DrawCircle(state.entities[i]->position.first, state.entities[i]->position.second, 3, RED);
    }
}
