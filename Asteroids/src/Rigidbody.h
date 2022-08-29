#pragma once

#include <SFML/Graphics.hpp>

class Rigidbody {

    // mass of the rigidbody
    int m_mass;

    // current velocity of the body
    sf::Vector2f m_velocity;

    // the max speed at which the body can travel
    float m_maxSpeed;

    // void clampVelocity();

    public:
    Rigidbody(int mass, float maxSpeed);

    // add external force to the body to influence its velocity
    void AddForce(sf::Vector2f force);

    // return current velocity
    sf::Vector2f GetVelocity();
    
};