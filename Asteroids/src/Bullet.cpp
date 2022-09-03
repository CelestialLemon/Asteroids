#include "Bullet.h"
#include "VectorHelper.h"
Bullet::Bullet()
: m_rb(Rigidbody(20, 1000)), m_fireForce(2000) {}

Bullet::Bullet(sf::Vector2f pos, sf::Vector2f dir, const sf::Texture& bulletTexture)
: m_rb(Rigidbody(20, 1000)), m_fireForce(2000)
{
    setSpriteTexture(bulletTexture);
    setPosition(pos);

    dir = Normalize(dir);
    m_rb.AddForce(m_fireForce * dir);
    
}

