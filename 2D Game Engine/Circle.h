
#pragma once

#include <SFML/Graphics.hpp>
#include "PhysicsBody.h"

class Circle
{
public:
    PhysicsBody physics;
    sf::CircleShape shape;

    Circle()
    {
        shape.setRadius(25.f);
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
