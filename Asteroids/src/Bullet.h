#pragma once

#include "Sprite.h"
#include "Rigidbody.h";
#include <SFML/Graphics.hpp>

class Bullet : public Sprite {
    Rigidbody m_rb;
    const float m_fireForce;

    // default-contstructor not allowed
    Bullet();
    public:
    // fire bullet from pos towards dir
    Bullet(sf::Vector2f pos, sf::Vector2f dir, const sf::Texture& bulletTexture);

    // update sprite position depending on current velocity, must be called every frame
    void updatePosition(float dt);

    // for polling input, running physics etc etc, must be called every frame
    void update(float dt, sf::RenderWindow& window) override;

    // checks if the bullet is in the playing area of the field
    bool isInBounds();
};