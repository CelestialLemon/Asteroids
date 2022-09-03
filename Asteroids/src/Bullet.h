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

    void updatePosition();
};