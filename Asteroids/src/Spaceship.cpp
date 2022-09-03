#include "Spaceship.h"

// initialize the spaceship params
Spaceship::Spaceship() 
: m_thrust(600000), m_rigidbody(Rigidbody(4000, 300))
{}

// update the position of the ship by calculating next position given currentPos and velocity and dt
void Spaceship::updatePosition(float dt) {
    
    sf::Vector2f currentVelocity = m_rigidbody.GetVelocity();

    float dy = currentVelocity.y * dt;
    float dx = currentVelocity.x * dt;

    Move(sf::Vector2f(dx, dy));


    // warp check
    auto currentPosition = getPosition();

    // 720.0f is the height and width of screen in pixels
    currentPosition.x = currentPosition.x < 0.0f ? 720.0f : currentPosition.x;
    currentPosition.x = currentPosition.x > 720.0f ? 0.0f : currentPosition.x;

    currentPosition.y = currentPosition.y < 0.0f ? 720.0f : currentPosition.y;
    currentPosition.y = currentPosition.y > 720.0f ? 0.0f : currentPosition.y;


    setPosition(currentPosition);
}

void Spaceship::updateRotation(float dt, sf::Vector2f mousePosition) {
    
    auto playerPosition = getPosition();
    // set player position to center of sprite
    playerPosition.x += 32.0f;
    playerPosition.y += 32.0f;

    // vector pointing from center of player to mouse
    auto mouseDirection = mousePosition - playerPosition;

    // normalize the direction
    float magnitude = sqrtf((mouseDirection.x * mouseDirection.x) + (mouseDirection.y * mouseDirection.y));
    
    mouseDirection.x /= magnitude;
    mouseDirection.y /= magnitude;
    //float magnitude = sqrtf((mouseDirection.x * mouseDirection.x) + (mouseDirection.y * mouseDirection.y));
    //mouseDirection /= magnitude;

    // find angle using x-cord
    float angle = acosf(mouseDirection.x);
    // convert angle to deg
    angle *= (180.0f / 3.14159f);

    // check quadrant
    if(mouseDirection.y < 0.0f) angle = 360.0f - angle;

    setRotation(angle);
}

// take input to move the ship
// TODO : normalize the movement vector so as to apply uniform thurst
void Spaceship::update(float dt, sf::RenderWindow& window) {
    
    // position
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        m_rigidbody.AddForce(sf::Vector2f(-m_thrust * dt, 0));
    
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        m_rigidbody.AddForce(sf::Vector2f(m_thrust * dt, 0));
    
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        m_rigidbody.AddForce(sf::Vector2f(0, -m_thrust * dt));
    
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        m_rigidbody.AddForce(sf::Vector2f(0, m_thrust * dt));

    updatePosition(dt);

    // rotation
    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
    updateRotation(dt, (sf::Vector2f)mousePosition);
}
