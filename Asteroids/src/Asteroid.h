#pragma once

#include <SFML/Graphics.hpp>
#include "Rigidbody.h"

class Asteroid {
    sf::ConvexShape m_asteroidShape;
    float m_radius;
    float m_currentHitpoints;
    float m_maxHitpoints;

    Rigidbody m_rigidbody;

    void SetPoints();
    void updatePosition(float dt);
    
    public:
    Asteroid();

    Asteroid(float radius, float maxHitpoints, int nPoints);
    void SetPosition(sf::Vector2f position);
    void draw(sf::RenderWindow& window);
    void update(float dt);

};