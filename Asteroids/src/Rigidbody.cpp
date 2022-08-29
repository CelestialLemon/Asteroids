#include "Rigidbody.h"

// initialize params, set initial speed to zero
Rigidbody::Rigidbody(int mass, float maxSpeed)
: m_mass(mass), m_maxSpeed(maxSpeed) {
    m_velocity = sf::Vector2f(0, 0);
}

/* 
sort of useless function
when speed goes above maxspeed it brings to to maxspeed
but because of the condition used in addForce method that if the body is already moving at maxSpeed skip
we don't need this
Depending on framerate the actual speed may exceed maxSpeed by a little but its better to leave it at that
than doing this calculation

Still gonna leave this function here, just in case
*/

/* void Rigidbody::clampVelocity() {
    float currentSpeed = sqrtf((m_velocity.x * m_velocity.x) + (m_velocity.y * m_velocity.y));

    if(currentSpeed <= m_maxSpeed) return;

    float scalingFactor = currentSpeed / m_maxSpeed;

    m_velocity.x *= scalingFactor;
    m_velocity.y *= scalingFactor;
} */


// take a vector2 force add calculate change in velocity
void Rigidbody::AddForce(sf::Vector2f force) {

    float currentSpeed = sqrtf((m_velocity.x * m_velocity.x) + (m_velocity.y * m_velocity.y));

    if(currentSpeed >= m_maxSpeed) return;

    float xAcceleration = force.x / m_mass;
    float yAcceleration = force.y / m_mass;

    m_velocity += sf::Vector2f(xAcceleration, yAcceleration);

    // clampVelocity();
}

// return current velocity of the body
sf::Vector2f Rigidbody::GetVelocity() {
    return m_velocity;
}