#include "oop/include/main_oop.h"
#include "config.h"
#include <algorithm>

void InitOOP(OOPState& state) {
    state.entities.clear();
    int initialCapacity = config.EntityCount;

    float initRadius = (float)config.CircleRadius;
    float padding = 2.0f;
    float cellSize = (initRadius * 2.0f) + padding;

    int cols = (int)(config.WINDOW_WIDTH / cellSize);
    int rows = (int)(config.WINDOW_HEIGHT / cellSize);

    if (cols <= 0) cols = 1;
    if (rows <= 0) rows = 1;

    int maxCapacity = cols * rows;

    if (initialCapacity > maxCapacity)
        initialCapacity = maxCapacity;

    std::vector<int> cellIndices(maxCapacity);
    for (int i = 0; i < maxCapacity; i++) {
        cellIndices[i] = i;
    }

    for (int i = maxCapacity - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        std::swap(cellIndices[i], cellIndices[j]);
    }

    for (auto i = 0; i < initialCapacity; i++) {
        int cellIndex = cellIndices[i];
        int col = cellIndex % cols;
        int row = cellIndex / cols;

        float baseX = (col * cellSize) + (cellSize / 2.0f);
        float baseY = (row * cellSize) + (cellSize / 2.0f);

        state.entities.push_back(std::make_unique<Circle>(
            baseX,
            baseY,
            true,
            rand() % 360));
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

    #pragma omp parallel for
    for (int i = 0; i < (int)state.entities.size(); i++) {
        for (auto& obstacle : state.obstacles) {
            obstacle->checkColision(state.entities[i].get());
        }
    }

    #pragma omp parallel for
    for (int i = 0; i < (int)state.entities.size(); i++) {
        state.entities[i]->movement(dt);
    }

    for (size_t i = 0; i < state.entities.size(); i++) {
        Color color = static_cast<Circle*>(state.entities[i].get())->infected ? GREEN : RED;
        DrawCircle(state.entities[i]->position.first, state.entities[i]->position.second, config.CircleRadius, color);
    }

    for (auto& obstacle : state.obstacles) {
        Color color = static_cast<Square*>(obstacle.get())->infected ? DARKGREEN : DARKGRAY;
        DrawRectangle(obstacle->position.first, obstacle->position.second, obstacle->size, obstacle->size, color);
    }
}
