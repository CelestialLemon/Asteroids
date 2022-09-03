#include "Bullet.h"

Bullet::Bullet()
: m_rb(Rigidbody(20, 1000)), m_fireForce(2000) {}

Bullet::Bullet(sf::Vector2f pos, sf::Vector2f dir, const sf::Texture& bulletTexture)
: m_rb(Rigidbody(20, 1000)), m_fireForce(50000)
{
    setSpriteTexture(bulletTexture);
    setPosition(pos);

    // normalize direction 
    float magnitude = sqrtf((dir.x * dir.x) + (dir.y * dir.y));
    
    dir.x /= magnitude;
    dir.y /= magnitude;


    m_rb.AddForce(m_fireForce * dir);
    
}

void Bullet::updatePosition(float dt) {
    auto currentVelocity = m_rb.GetVelocity();

    float dy = currentVelocity.y * dt;
    float dx = currentVelocity.x * dt;

    Move(sf::Vector2f(dx, dy));
}

// bullets only need update position
void Bullet::update(float dt, sf::RenderWindow& window) {
    updatePosition(dt);
}

// returns true if the bullet is in playing area
bool Bullet::isInBounds() {
    auto currentPosition = getPosition();

    if(currentPosition.x < 0 || currentPosition.x > 720) return false;
    if(currentPosition.y < 0 || currentPosition.y > 720) return false;

    return true;
}

