#include "Bullet.h"

const float BULLET_FIRE_FORCE = 50000;
const float BULLET_RIGIDBODY_MASS = 20;
const float BULLET_RIGIDBODY_MAX_SPEED = 1000;

Bullet::Bullet()
: m_rb(Rigidbody(BULLET_RIGIDBODY_MASS, BULLET_RIGIDBODY_MAX_SPEED)), m_fireForce(BULLET_FIRE_FORCE) {}

Bullet::Bullet(sf::Vector2f pos, sf::Vector2f dir, const sf::Texture& bulletTexture)
: m_rb(Rigidbody(BULLET_RIGIDBODY_MASS, BULLET_RIGIDBODY_MAX_SPEED)), m_fireForce(BULLET_FIRE_FORCE)
{
    // the bullet texture is only loaded once in the Spaceship class
    // all bullets use this same texture reference to avoid loading on firing every bullet
    setSpriteTexture(bulletTexture);

    setPosition(pos);

    // normalize direction 
    const float magnitude = sqrtf((dir.x * dir.x) + (dir.y * dir.y));
    
    dir.x /= magnitude;
    dir.y /= magnitude;

    // fire
    m_rb.AddForce(m_fireForce * dir);
}

void Bullet::updatePosition(float dt) {
    auto currentVelocity = m_rb.GetVelocity();

    const float dy = currentVelocity.y * dt;
    const float dx = currentVelocity.x * dt;

    Move(sf::Vector2f(dx, dy));
}

// bullets only need update position
void Bullet::update(float dt, sf::RenderWindow& window) {
    updatePosition(dt);
}

// returns true if the bullet is in playing area
bool Bullet::isInBounds() {
    const auto currentPosition = getPosition();

    if(currentPosition.x < 0 || currentPosition.x > 720) return false;
    if(currentPosition.y < 0 || currentPosition.y > 720) return false;

    return true;
}

