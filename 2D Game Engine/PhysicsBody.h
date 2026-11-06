
#pragma once

class PhysicsBody
{
public:
    float velocityX = 0.f;
    float velocityY = 0.f;

    float gravity = 9.81f * 100.f;

    void Update(float deltaTime)
    {
        velocityY -= gravity * deltaTime;
    }
};
