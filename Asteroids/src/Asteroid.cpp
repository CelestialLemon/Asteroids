#include "Asteroid.h"

float randomFloat() {
    // returns random float between 0.5 and 0.99
    int r = rand() % 50 + 50;
    return r / 100.0f;
}

Asteroid::Asteroid() : 
m_radius(50), 
m_currentHitpoints(100), 
m_maxHitpoints(100), 
m_rigidbody(Rigidbody(100 * 1000, 500))
{
    // default number of points 10
    m_asteroidShape.setPointCount(10);
    m_asteroidShape.setOutlineColor(sf::Color::White);
    m_asteroidShape.setOutlineThickness(3);
    m_asteroidShape.setFillColor(sf::Color::Black);

    m_rigidbody.AddForce(sf::Vector2f(50000, 0));

    SetPoints();
}

Asteroid::Asteroid(float radius, float maxHitpoints, int nPoints) : 
m_radius(radius), 
m_currentHitpoints(maxHitpoints),
m_maxHitpoints(maxHitpoints),
// mass of the asteroid is directly propertional to its maximum hitpoints
m_rigidbody(Rigidbody(maxHitpoints * 1000, 500))
{
    // TODO : change asteroid border thickness by its hitpoints
    m_asteroidShape.setPointCount(nPoints);
    m_asteroidShape.setOutlineColor(sf::Color::White);
    m_asteroidShape.setOutlineThickness(3);
    m_asteroidShape.setFillColor(sf::Color::Black);

    m_rigidbody.AddForce(sf::Vector2f(2500000, 0));

    SetPoints();
}

void Asteroid::SetPoints() {
    
    int nPoints = m_asteroidShape.getPointCount();

    float angleIncrease = 360.0f / nPoints;

    int index = 0;
    float angle = 0;

    for(int index = 0; index < nPoints; index++) {
        float dist = randomFloat() * m_radius;

        float x = dist * cos(angle * 3.14159 / 180);
        float y = dist * sin(angle * 3.14159 / 180);

        m_asteroidShape.setPoint(index, sf::Vector2f(x, y));

        angle += angleIncrease;
    }
}

void Asteroid::SetPosition(sf::Vector2f position) {
    m_asteroidShape.setPosition(position);
}

void Asteroid::draw(sf::RenderWindow& window) {
    window.draw(m_asteroidShape);
}

void Asteroid::updatePosition(float dt) {
    auto currentVelocity = m_rigidbody.GetVelocity();

    sf::Vector2f displacement = sf::Vector2f(currentVelocity.x * dt, currentVelocity.y * dt);

    auto currentPosition = m_asteroidShape.getPosition();
    m_asteroidShape.setPosition(currentPosition + displacement);
}

void Asteroid::update(float dt) {
    updatePosition(dt);
}


