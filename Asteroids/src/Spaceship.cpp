#include "Spaceship.h"

// initialize the spaceship params
Spaceship::Spaceship() 
: m_thrust(300000), m_rigidbody(Rigidbody(4000, 300))
{}

// update the position of the ship by calculating next position given currentPos and velocity and dt
void Spaceship::updatePosition(float dt) {
    
    sf::Vector2f currentVelocity = m_rigidbody.GetVelocity();

    float currentSpeed = sqrtf((currentVelocity.x * currentVelocity.x) + (currentVelocity.y * currentVelocity.y));
    printf("Speed: %.2f\n", currentSpeed);

    float dy = currentVelocity.y * dt;
    float dx = currentVelocity.x * dt;

    Move(sf::Vector2f(dx, dy));
}

// take input to move the ship
// TODO : normalize the movement vector so as to apply uniform thurst
void Spaceship::pollInput(float dt) {
    
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        m_rigidbody.AddForce(sf::Vector2f(-m_thrust * dt, 0));
    
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        m_rigidbody.AddForce(sf::Vector2f(m_thrust * dt, 0));
    
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        m_rigidbody.AddForce(sf::Vector2f(0, -m_thrust * dt));
    
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        m_rigidbody.AddForce(sf::Vector2f(0, m_thrust * dt));

    updatePosition(dt);
}