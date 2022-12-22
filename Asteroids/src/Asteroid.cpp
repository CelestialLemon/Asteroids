#include "Asteroid.h"
#include <iostream>

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

// spawn asteroid at a random point on a circle with c = 0, r = 1500.0f
void Asteroid::SetRandomSpawnPosition() {
    // get a random angle from 0 - 360
    float angle = rand() % 360;

    // radius of the circle is 1500
    const float spawnDistance = 1500.0f;

    // calculate position of the point using angle and distance
    sf::Vector2f spawnPoint = sf::Vector2f(spawnDistance * cos(angle * 3.14159 / 180), spawnDistance * sin(angle * 3.14159 / 180));
    
    // set position to asteroid
    m_asteroidShape.setPosition(spawnPoint);
}

// default contructor
Asteroid::Asteroid() :

// default max radius for asteroid 50 pixels
m_radius(50), 

// default spawn hitpoints for asteroid 100
m_currentHitpoints(100), 
m_maxHitpoints(100),

// default size for asteroid is large
m_asteroidSize(AsteroidSize::LARGE),

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

    SetRandomSpawnPosition();
    sf::Vector2f hurlDestination(rand() % 720, rand() % 720);
    Hurl(hurlDestination);
}

// parameterized contructor, same as default execpt for user defined values
Asteroid::Asteroid(AsteroidSize asteroidSize) : 
m_asteroidSize(asteroidSize)
// mass of the asteroid is directly propertional to its maximum hitpoints
{
    // number of points in the polygon of the asteroid
    int nPoints = 0;
    // TODO : change asteroid border thickness by its hitpoints
    if(asteroidSize == AsteroidSize::SMALL) {
        m_radius = rand() % 25 + 25;
        nPoints = rand() % 5 + 5;
    }
    else if(asteroidSize == AsteroidSize::MEDIUM) {
        m_radius = rand() % 25 + 50;
        nPoints = rand() % 5 + 10;
    }
    else {
        // asteroidSize == AsteroidSize::LARGE
        m_radius = rand() % 25 + 75;
        nPoints = rand() % 5 + 15;
    }

    // the larger that asteroid radius the greater its health
    // will add some multiplier here for balancing
    m_maxHitpoints = m_radius;
    m_currentHitpoints = m_radius;
    m_rigidbody = Rigidbody(m_maxHitpoints * 1000, 500);
    std::cout << "hitpoints: " << m_maxHitpoints << '\n';
    m_asteroidShape.setPointCount(nPoints);
    SetPoints();
    
    m_asteroidShape.setOutlineColor(sf::Color::White);
    m_asteroidShape.setOutlineThickness(3);
    m_asteroidShape.setFillColor(sf::Color::Black);

    SetRandomSpawnPosition();
    sf::Vector2f hurlDestination(rand() % 720, rand() % 720);
    Hurl(hurlDestination);

}

Asteroid::Asteroid(sf::Vector2f position, AsteroidSize asteroidSize, sf::Vector2f hurlDestination) : 
m_asteroidSize(asteroidSize)
// mass of the asteroid is directly propertional to its maximum hitpoints
{
    // number of points in the polygon of the asteroid
    int nPoints = 0;
    // TODO : change asteroid border thickness by its hitpoints
    if(asteroidSize == AsteroidSize::SMALL) {
        m_radius = rand() % 25 + 25;
        nPoints = rand() % 5 + 5;
    }
    else if(asteroidSize == AsteroidSize::MEDIUM) {
        m_radius = rand() % 25 + 50;
        nPoints = rand() % 5 + 10;
    }
    else {
        // asteroidSize == AsteroidSize::LARGE
        m_radius = rand() % 25 + 75;
        nPoints = rand() % 5 + 15;
    }

    // the larger that asteroid radius the greater its health
    // will add some multiplier here for balancing
    m_maxHitpoints = m_radius;
    m_currentHitpoints = m_radius;
    m_rigidbody = Rigidbody(m_maxHitpoints * 1000, 500);
    m_asteroidShape.setPointCount(nPoints);
    SetPoints();
    
    m_asteroidShape.setOutlineColor(sf::Color::White);
    m_asteroidShape.setOutlineThickness(3);
    m_asteroidShape.setFillColor(sf::Color::Black);
    m_asteroidShape.setPosition(position);

    Hurl(hurlDestination);
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

void Asteroid::draw(sf::RenderWindow& window) const {
    window.draw(m_asteroidShape);
}

void Asteroid::Hurl(sf::Vector2f destination) {

    const sf::Vector2f spawnPoint = m_asteroidShape.getPosition();
    // destination of asteroid is a random point on the screen;

    sf::Vector2f forceDir = destination - spawnPoint;

    normalize(forceDir);

    // TODO: Hurl asteroids with different forces
    forceDir.x *= 25000000;
    forceDir.y *= 25000000;

    m_rigidbody.AddForce(forceDir);
}

float operator* (sf::Vector2f left, sf::Vector2f right) {
    return (left.x * right.x) + (left.y * right.y);
}

bool pointTriangleIntersection(sf::Vector2f p, sf::Vector2f a, sf::Vector2f b, sf::Vector2f c) {
    
    bool b0 = (sf::Vector2f(p.x - a.x, p.y - a.y) * sf::Vector2f(a.y - b.y, b.x - a.x) > 0);
    bool b1 = (sf::Vector2f(p.x - b.x, p.y - b.y) * sf::Vector2f(b.y - c.y, c.x - b.x) > 0);
    bool b2 = (sf::Vector2f(p.x - c.x, p.y - c.y) * sf::Vector2f(c.y - a.y, a.x - c.x) > 0);

    return (b0 == b1 && b1 == b2);
}

bool Asteroid::IsPointInside(sf::Vector2f point) const {

    int nPoints = m_asteroidShape.getPointCount();

    for(int i = 0; i < nPoints - 1; i++) {
        sf::Vector2f pointA = m_asteroidShape.getPoint(i) + m_asteroidShape.getPosition();
        sf::Vector2f pointB = m_asteroidShape.getPoint(i + 1) + m_asteroidShape.getPosition();
        sf::Vector2f pointC = m_asteroidShape.getPosition();

        bool isInTriangle = pointTriangleIntersection(point, pointA, pointB, pointC);

        if(isInTriangle) return true;
    }

    sf::Vector2f pointA = m_asteroidShape.getPoint(nPoints - 1) + m_asteroidShape.getPosition();
    sf::Vector2f pointB = m_asteroidShape.getPoint(0) + m_asteroidShape.getPosition();
    sf::Vector2f pointC = m_asteroidShape.getPosition();

    bool isInTriangle = pointTriangleIntersection(point, pointA, pointB, pointC);

    if(isInTriangle) return true;
    else return false;
}

void Asteroid::Hit(float damage) {
    m_currentHitpoints -= damage;
}

float Asteroid::GetHitpoints() const {
    return m_currentHitpoints;
}

sf::Vector2f Asteroid::GetVelocity() const {
    return m_rigidbody.GetVelocity();
}

AsteroidSize Asteroid::GetAsteroidSize() const {
    return m_asteroidSize;
}

sf::Vector2f Asteroid::GetPosition() const {
    return m_asteroidShape.getPosition();
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

bool Asteroid::isInBounds() const {
    auto currentPosition = m_asteroidShape.getPosition();

    float distanceFromOrigin = sqrtf((currentPosition.x * currentPosition.x) + (currentPosition.y * currentPosition.y));

    // asteroid should not exist outside 3000 units from origin
    if(distanceFromOrigin >= 3000) return false;
    else return true;
}


