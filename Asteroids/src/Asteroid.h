#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_set>
#include "Rigidbody.h"

enum AsteroidSize {
    SMALL = 0,
    MEDIUM = 1,
    LARGE = 2
};

class Asteroid {

    // convex shape of the asteroids that is also the sprite for the object
    sf::ConvexShape m_asteroidShape;

    AsteroidSize m_asteroidSize;

    // radius which will be used while randomly generating vertices for the convex shape of asteroid
    float m_radius;

    // current hitpoints of a given asteroid instance
    float m_currentHitpoints;

    // maximum hitpoints of a given asteroid instance (asteroids spawn with these hitpoints)
    float m_maxHitpoints;

    // rigidbody to control physics and motion object
    Rigidbody m_rigidbody;

    // proceduraly generate vertices for the convex shape of the object
    void SetPoints();

    // update position depending on velocity given by rigidbody
    void updatePosition(float dt);

    // throw the asteroid towards given point
    void Hurl(sf::Vector2f destination, float force);

    void SetRandomSpawnPosition();


    public:
    // default contructor with default values
    Asteroid();

    // parameterized contructors for setting custom values of following properties
    // this contructor is used to create a asteroid on spawn ring
    Asteroid(AsteroidSize asteroidSize);

    // this paramertized contructor is used to create asteroid when bigger ones are destroyed
    Asteroid(sf::Vector2f position, AsteroidSize asteroidSize, sf::Vector2f hurlDirection, float force);

    // explicitly set the position of asteroid, should only be set when a new asteroid is instantiated
    void SetPosition(sf::Vector2f position);

    // draw the convex shape of the asteroid to given window
    void draw(sf::RenderWindow& window) const;

    bool IsPointInside(sf::Vector2f point) const;

    void Hit(float damage);

    float GetHitpoints() const;
    AsteroidSize GetAsteroidSize() const;
    sf::ConvexShape GetConvexShape() const;
    sf::Vector2f GetPosition() const;
    float GetRotation() const;
    sf::Vector2f GetVelocity() const;
    // returns if the asteroids is the permitted bounds or not
    bool isInBounds() const;

    // update function is called in application every frame use it for physics taking input etc.
    void update(float dt);

};