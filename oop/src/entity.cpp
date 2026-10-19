#include "entity.h"

Entity::Entity(float x, float y, bool canMove, float directionVector) {
    position = std::make_pair(x, y);
    this->canMove = canMove;
    this->directionVector = directionVector;
}

void Entity::movement(float dt) {
}

Shape::Shape(float x, float y, bool canMove, float directionVector)
    : Entity(x, y, canMove, directionVector) {
}

Circle::Circle(float x, float y, bool canMove, float directionVector)
    : Shape(x, y, canMove, directionVector), radius(0.0f), infected(false) {
}

void Circle::checkColision() {
}

void Circle::movement(float dt) {
    if (!canMove) return;
    float speed = 100.0f;
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

Square::Square(float x, float y, bool canMove, float directionVector) : Shape(x, y, canMove, directionVector), size(0.0f) { }
