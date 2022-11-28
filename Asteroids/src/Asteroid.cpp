#include "Asteroid.h"

static float randomFloat() {
    // returns random float between 0.5 and 0.99
    int r = rand() % 50 + 50;
    return r / 100.0f;
}

static void normalize(sf::Vector2f& vec) {
    float magnitude = sqrtf((vec.x * vec.x) + (vec.y * vec.y));

    vec.x /= magnitude;
    vec.y /= magnitude;
}

// default contructor
Asteroid::Asteroid() :

// default max radius for asteroid 50 pixels
m_radius(50), 

// default spawn hitpoints for asteroid 100
m_currentHitpoints(100), 
m_maxHitpoints(100),

// mass is directly proportional to hitpoints i.e mass = 1000 * hitpoints, and maxspeed for asteroid is 500
m_rigidbody(Rigidbody(100 * 1000, 500))
{
    // default number of points 10
    m_asteroidShape.setPointCount(10);
    // construct the shape of the asteroid by generating vertices
    SetPoints();

    // set outline color as white and fill color as black
    m_asteroidShape.setOutlineColor(sf::Color::White);
    m_asteroidShape.setOutlineThickness(3);
    m_asteroidShape.setFillColor(sf::Color::Black);

    Hurl();
}

// parameterized contructor, same as default execpt for user defined values
Asteroid::Asteroid(float radius, float maxHitpoints, int nPoints) : 
m_radius(radius), 
m_currentHitpoints(maxHitpoints),
m_maxHitpoints(maxHitpoints),
// mass of the asteroid is directly propertional to its maximum hitpoints
m_rigidbody(Rigidbody(maxHitpoints * 1000, 500))
{
    // TODO : change asteroid border thickness by its hitpoints
    m_asteroidShape.setPointCount(nPoints);
    SetPoints();
    
    m_asteroidShape.setOutlineColor(sf::Color::White);
    m_asteroidShape.setOutlineThickness(3);
    m_asteroidShape.setFillColor(sf::Color::Black);

    Hurl();

}

// generate vertices for shape of asteroid
void Asteroid::SetPoints() {
    
    int nPoints = m_asteroidShape.getPointCount();

    /*
        HOW THIS WORKS
        Start with 0 degrees and with random float determine the distance of the vertex from the center
        Then calculate the x and y co-ordinate using angle and distance i.e convert polar to cartesian
        Set the point in the convex shape
        Increase the angle and repeat

        So if we have 10 points we will generate vertices that lie at
        0 36 72 108 144 180 ... 324
        All these angles will have point at varying distances so it gives the asteroid a jagged look insted of a 
        smooth circle / polygon
    */
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

void Asteroid::Hurl() {
    float angle = rand() % 360;
    const float spawnDistance = 1500.0f;
    sf::Vector2f spawnPoint = sf::Vector2f(spawnDistance * cos(angle * 3.14159 / 180), spawnDistance * sin(angle * 3.14159 / 180));
    
    m_asteroidShape.setPosition(spawnPoint);

    // destination of asteroid is a random point on the screen;
    sf::Vector2f destination = sf::Vector2f(rand() % 720, rand() % 720);

    sf::Vector2f forceDir = destination - spawnPoint;

    normalize(forceDir);

    // TODO: Hurl asteroids with different forces
    forceDir.x *= 25000000;
    forceDir.y *= 25000000;

    m_rigidbody.AddForce(forceDir);
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

bool Asteroid::isInBounds() {
    auto currentPosition = m_asteroidShape.getPosition();

    float distanceFromOrigin = sqrtf((currentPosition.x * currentPosition.x) + (currentPosition.y * currentPosition.y));

    // asteroid should not exist outside 3000 units from origin
    if(distanceFromOrigin >= 3000) return false;
    else return true;
}


