#include "oop/include/main_oop.h"
#include "config.h"
#include <algorithm>

void InitOOP(OOPState& state) {
    state.entities.clear();
    for (auto i = 0; i < config.EntityCount; i++) {
        state.entities.push_back(std::make_unique<Circle>(rand() % config.WINDOW_WIDTH, rand() % config.WINDOW_HEIGHT, true, rand() % 360));
    }
    if (config.Virus && !state.entities.empty()) {
        static_cast<Circle*>(state.entities[0].get())->infected = true;
    }
    state.obstacles.clear();
    for (auto i = 0; i < config.ObstacleCount; i++) {
        state.obstacles.push_back(std::make_unique<Square>(config.Obstacles[i][0], config.Obstacles[i][1], false, 0));
    }
    state.isInitialized = true;
}

void oop(OOPState& state, float dt) {
    if (!state.isInitialized) {
        InitOOP(state);
    }

    std::erase_if(state.entities, [](const std::unique_ptr<Entity>& e) {
        auto* c = static_cast<Circle*>(e.get());
        return c->infected && c->infectedTime >= config.VirusLifetime;
    });

    float cellSize = 2.0f * config.CircleRadius;
    int cols = static_cast<int>(config.WINDOW_WIDTH / cellSize) + 1;
    int rows = static_cast<int>(config.WINDOW_HEIGHT / cellSize) + 1;
    std::vector<std::vector<size_t>> grid(cols * rows);
    std::vector<int> cellOf(state.entities.size());

    for (size_t i = 0; i < state.entities.size(); i++) {
        int cx = std::clamp(static_cast<int>(state.entities[i]->position.first / cellSize), 0, cols - 1);
        int cy = std::clamp(static_cast<int>(state.entities[i]->position.second / cellSize), 0, rows - 1);
        cellOf[i] = cy * cols + cx;
        grid[cellOf[i]].push_back(i);
    }

    for (size_t j = 0; j < state.entities.size(); j++) {
        int cx = cellOf[j] % cols;
        int cy = cellOf[j] / cols;
        for (int ny = cy - 1; ny <= cy + 1; ny++) {
            for (int nx = cx - 1; nx <= cx + 1; nx++) {
                if (nx < 0 || nx >= cols || ny < 0 || ny >= rows) continue;
                for (size_t k : grid[ny * cols + nx]) {
                    if (k <= j) continue;
                    state.entities[j]->checkColision(state.entities[k].get());
                }
            }
        }
    }

    for (auto& obstacle : state.obstacles) {
        for (auto& entity : state.entities) {
            obstacle->checkColision(entity.get());
        }
    }

    for (size_t i = 0; i < state.entities.size(); i++) {
        state.entities[i]->movement(dt);
        Color color = static_cast<Circle*>(state.entities[i].get())->infected ? GREEN : RED;
        DrawCircle(state.entities[i]->position.first, state.entities[i]->position.second, config.CircleRadius, color);
    }

    for (auto& obstacle : state.obstacles) {
        DrawRectangle(obstacle->position.first, obstacle->position.second, obstacle->size, obstacle->size, DARKGRAY);
    }
}
