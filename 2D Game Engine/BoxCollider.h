
#pragma once

#include <SFML/Graphics.hpp>

class BoxCollider
{
private:
    sf::FloatRect bounds;

public:

    // Constructor
    BoxCollider(
        sf::Vector2f position = { 0.f, 0.f },
        sf::Vector2f size = { 0.f, 0.f }
    )
    {
        bounds.position = position;
        bounds.size = size;
    }

    // Set collider position
    void SetPosition(sf::Vector2f position)
    {
        bounds.position = position;
    }

    // Set collider size
    void SetSize(sf::Vector2f size)
    {
        bounds.size = size;
    }

    // Get collider position
    sf::Vector2f GetPosition() const
    {
        return bounds.position;
    }

    // Get collider size
    sf::Vector2f GetSize() const
    {
        return bounds.size;
    }

    // Get complete collider bounds
    sf::FloatRect GetBounds() const
    {
        return bounds;
    }

    // Check collision with another BoxCollider
    bool CheckCollision(const BoxCollider& other) const
    {
        return bounds.findIntersection(other.bounds).has_value();
    }

    // Check collision with an SFML rectangle
    bool CheckCollision(const sf::FloatRect& other) const
    {
        return bounds.findIntersection(other).has_value();
    }

    // Move collider by an offset
    void Move(sf::Vector2f offset)
    {
        bounds.position += offset;
    }

    // Reset collider
    void Reset()
    {
        bounds.position = { 0.f, 0.f };
        bounds.size = { 0.f, 0.f };
    }
};
