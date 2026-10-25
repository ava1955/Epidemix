#pragma once
#include <utility>
#include <cmath>
#include <raylib.h>
#include <vector>

class Entity {
public:
    std::pair<float, float> position;
    bool canMove;
    float directionVector;
    Entity(float x, float y, bool canMove, float directionVector);
    virtual ~Entity() = default;
    virtual void movement(float dt);
    virtual void checkColision(Entity* a);
};

class Shape : public Entity {
public:
    Shape(float x, float y, bool canMove, float directionVector);
};

class Circle : public Shape {
public:
    float radius;
    bool infected;
    float infectedTime;

    Circle(float x, float y, bool canMove, float directionVector);

    void checkColision(Entity* a) override;
    void movement(float dt) override;
};

class Square : public Shape {
public:
    float size;

    Square(float x, float y, bool canMove, float directionVector);

    void checkColision(Entity* a) override;
};
