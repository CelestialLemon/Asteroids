#include "Sprite.h"
#include "Rigidbody.h"

#include <SFML/Graphics.hpp>

// The player class
// Inherits from Sprite because it has a sprite
class Spaceship : public Sprite {
    
    // use to performs physics on the object
    Rigidbody m_rigidbody;
    
    // amount of force applied to the rigidbody per second
    float m_thrust;

    public:
    Spaceship();

    // update the position of the ship by calculating next position given currentPos and velocity and dt
    void updatePosition(float dt);

    // update the rotation depending on mouse position
    void updateRotation(float dt, sf::Vector2f mousePosition);
    
    // take input from user to move the spaceship
    void pollInput(float dt, sf::RenderWindow& window);
};