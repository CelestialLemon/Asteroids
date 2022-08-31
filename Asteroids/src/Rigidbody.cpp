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

EDIT: Turns out I do need this function after all
If i just dont take input after player reaches maxspeed, then we can't slow down
or do anything after reaching maxspeed

So im adding this back
But there is a problem. While this function works, it does so by scaling the speed
to maxSpeed if it exceeds the maxSpeed. What this means is that instead of stopping 
taking input on the axes to velocity, it decays both velocity and takes input on the axes

e.g. if maxSpeed = 300, xVelocity is 220 and yVelocity is 203, the spaceship is at maxSpeed
so instead of stopping taking input on +ve x and y axes, it takes the input, increases the velocity
then scales and brings it down to 300. This is not ideal in a physics engine as it means that
if player pressed right we take the input and increase xVelocity, but we scale down both the
xVelocity and yVelocity. It means that despite not pressing down, the yVelocity begins to decay.

But I actually like the effect here as it keeps increasing the speed in direction the player
wants at the cost of yVelocity. This makes it seems like the player can still control the ship
without exceeding max speed.
*/

void Rigidbody::clampVelocity() {
    float currentSpeed = sqrtf((m_velocity.x * m_velocity.x) + (m_velocity.y * m_velocity.y));

    if(currentSpeed <= m_maxSpeed) return;

    float scalingFactor = m_maxSpeed / currentSpeed;

    m_velocity.x *= scalingFactor;
    m_velocity.y *= scalingFactor;
}


// take a vector2 force add calculate change in velocity
void Rigidbody::AddForce(sf::Vector2f force) {

    float currentSpeed = sqrtf((m_velocity.x * m_velocity.x) + (m_velocity.y * m_velocity.y));

    float xAcceleration = force.x / m_mass;
    float yAcceleration = force.y / m_mass;

    m_velocity += sf::Vector2f(xAcceleration, yAcceleration);

    clampVelocity();
}

// return current velocity of the body
sf::Vector2f Rigidbody::GetVelocity() {
    return m_velocity;
}