#include "oop/include/entity.h"
#include "config.h"
#include <algorithm>

Entity::Entity(float x, float y, bool canMove, float directionVector) {
    position = std::make_pair(x, y);
    this->canMove = canMove;
    this->directionVector = directionVector;
}

void Entity::movement(float dt) {
}

void Entity::checkColision(Entity* a) {
}

Shape::Shape(float x, float y, bool canMove, float directionVector)
    : Entity(x, y, canMove, directionVector) {
}

Circle::Circle(float x, float y, bool canMove, float directionVector)
    : Shape(x, y, canMove, directionVector), radius(static_cast<float>(config.CircleRadius)), infected(false), infectedTime(0.0f) {
}

void Circle::checkColision(Entity* a) {
    float dx = this->position.first - a->position.first;
    float dy = a->position.second - this->position.second;
    float r = 2.0f * config.CircleRadius;

    if (dx * dx + dy * dy < r * r) {
        float tmp;

        tmp = a->directionVector; a->directionVector = this->directionVector; this->directionVector = tmp;

        auto* other = static_cast<Circle*>(a);
        infected = other->infected = infected || other->infected;
    }
}

void Circle::movement(float dt) {
    if (infected) infectedTime += dt;
    if (!canMove) return;
    float speed = config.CircleSpeed;
    float rad = directionVector * DEG2RAD;

    position.first += std::cos(rad) * speed * dt;
    position.second += std::sin(rad) * speed * dt;

    float w = static_cast<float>(GetScreenWidth());
    float h = static_cast<float>(GetScreenHeight());

    if (position.first < radius) {
        position.first = radius;
        directionVector = 180.0f - directionVector;
    }
    else if (position.first > w - radius) {
        position.first = w - radius;
        directionVector = 180.0f - directionVector;
    }

    if (position.second < radius) {
        position.second = radius;
        directionVector = -directionVector;
    }
    else if (position.second > h - radius) {
        position.second = h - radius;
        directionVector = -directionVector;
    }

    if (directionVector < 0.0f)   directionVector += 360.0f;
    if (directionVector >= 360.0f) directionVector -= 360.0f;
}

Square::Square(float x, float y, bool canMove, float directionVector) : Shape(x, y, canMove, directionVector), size(static_cast<float>(config.ObstacleSize)) { }

void Square::checkColision(Entity* a) {
    float r = config.CircleRadius;
    float nearestX = std::clamp(a->position.first, position.first, position.first + size);
    float nearestY = std::clamp(a->position.second, position.second, position.second + size);
    float dx = a->position.first - nearestX;
    float dy = a->position.second - nearestY;

    if (dx * dx + dy * dy < r * r) {
        if (std::fabs(dx) > std::fabs(dy)) {
            a->position.first = dx > 0 ? position.first + size + r : position.first - r;
            a->directionVector = 180.0f - a->directionVector;
        }
        else {
            a->position.second = dy > 0 ? position.second + size + r : position.second - r;
            a->directionVector = -a->directionVector;
        }
    }
}
