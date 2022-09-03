#include "Bullet.h"

Bullet::Bullet()
: m_rb(Rigidbody(20, 1000)), m_fireForce(2000) {}

Bullet::Bullet(sf::Vector2f pos, sf::Vector2f dir, const sf::Texture& bulletTexture)
: m_rb(Rigidbody(20, 1000)), m_fireForce(10000)
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

void Bullet::update(float dt, sf::RenderWindow& window) {
    updatePosition(dt);
}

