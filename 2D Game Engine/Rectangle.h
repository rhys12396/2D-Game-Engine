
#pragma once

#include <SFML/Graphics.hpp>
#include "PhysicsBody.h"

class Rectangle
{
public:
    PhysicsBody physics;
    sf::RectangleShape shape;

    Rectangle()
    {
        shape.setSize({50.f, 50.f});
        shape.setFillColor(sf::Color::Red);
    }

    void Update(float deltaTime)
    {
        physics.Update(deltaTime);

        shape.move({
            physics.velocityX * deltaTime,
            physics.velocityY * deltaTime
        });
    }

    void Draw(sf::RenderTarget& target)
    {
        target.draw(shape);
    }
};
